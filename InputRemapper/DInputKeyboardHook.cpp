
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Shlwapi.h>
#include <TlHelp32.h>

#include "../shared/shared_data.h"
#include "DInputKeyboardHook.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shlwapi.lib")


#define	IS_DATA_LOCKED()	(m_Lock != 0)
#define	LOCK_DATA()			while(_InterlockedExchange(&m_Lock, 1))
#define	UNLOCK_DATA()		_InterlockedExchange(&m_Lock, 0)


BOOL InjectModuleW(DWORD ProcessId, PWSTR ModulePath)
{
	PVOID NameBuffer;
	ULONG BufferLength;
	ULONG BytesWritten;
	PVOID FnLoadLibraryW;
	HANDLE ProcessHandle;
	HANDLE ThreadHandle;
	DWORD ThreadId;
	DWORD BaseAddress;

	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (!ProcessHandle)
		return FALSE;

	BufferLength = (wcslen(ModulePath) + 1) * sizeof(WCHAR);
	NameBuffer = VirtualAllocEx(ProcessHandle, NULL, BufferLength, MEM_COMMIT, PAGE_READWRITE);

	if (!WriteProcessMemory(ProcessHandle, NameBuffer, ModulePath, BufferLength, &BytesWritten) ||
		BytesWritten != BufferLength)
	{
		VirtualFreeEx(ProcessHandle, NameBuffer, 0, MEM_FREE);
		CloseHandle(ProcessHandle);
		return FALSE;
	}

	FnLoadLibraryW = (PVOID)GetProcAddress(LoadLibrary(TEXT("kernel32.dll")), "LoadLibraryW");
	ThreadHandle = CreateRemoteThread(ProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)FnLoadLibraryW, NameBuffer, 0, &ThreadId);

	if (!ThreadHandle)
	{
		VirtualFreeEx(ProcessHandle, NameBuffer, 0, MEM_FREE);
		CloseHandle(ProcessHandle);
		return FALSE;
	}

	WaitForSingleObject(ThreadHandle, INFINITE);

	BaseAddress = 0;
	GetExitCodeThread(ThreadHandle, &BaseAddress);

	VirtualFreeEx(ProcessHandle, NameBuffer, 0, MEM_FREE);
	CloseHandle(ThreadHandle);

	CloseHandle(ProcessHandle);

	return BaseAddress != 0;
}

BOOL GetRemapperModulePathW(WCHAR *Path, DWORD BufferLength)
{
	WCHAR ModulePath[MAX_PATH];

	GetModuleFileNameW(GetModuleHandle(NULL), ModulePath, _countof(ModulePath));
	PathFindFileNameW(ModulePath)[0] = 0;
	PathAppendW(ModulePath, DIHOOK_REMAPPER_CORE_DLL_NAME);

	if (BufferLength > wcslen(ModulePath) + 1)
	{
		wcscpy_s(Path, BufferLength, ModulePath);
		return TRUE;
	}

	return FALSE;
}



CDInputKeyboardHook::CDInputKeyboardHook() :
	m_SharedData(nullptr), m_SelfMutexHandle(nullptr), m_MappingHandle(nullptr), m_ThreadHandle(nullptr),
	m_Lock(0)
{

}

CDInputKeyboardHook::~CDInputKeyboardHook()
{
	Shutdown();
}

DWORD WINAPI CDInputKeyboardHook::ServerThread(IN CDInputKeyboardHook *This)
{
	__try
	{
		while (!(This->m_SharedData->Flags & DIHOOK_FLAG_SHUTDOWN))
		{
			Sleep(500);
			This->m_SharedData->Heartbeat++;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// Exception!
	}

	return 0;
}

VOID CDInputKeyboardHook::InitializeSharedData(struct _DIHOOK_SHARED_DATA *SharedData)
{
	SharedData->Flags = 0;
	SharedData->Heartbeat = 0;

	for (INT i = 0; i < _countof(SharedData->KeyRemapping); i++)
		SharedData->KeyRemapping[i] = (BYTE)i;
}

BOOL CDInputKeyboardHook::Initialize()
{
	WCHAR MutexName[100];
	HANDLE hMutex = NULL;
	HANDLE hFileMapping = NULL;
	PVOID SharedMemory = NULL;
	HANDLE hThread = NULL;
	BOOLEAN Initialized = FALSE;

	//	ZeroMemory(Context, sizeof(*Context));

	do
	{
		//
		// Create Our Mutex.
		//

		wsprintf(MutexName, DIHOOK_MUTEX_NAME, GetCurrentProcessId());
		hMutex = CreateMutex(NULL, FALSE, MutexName);

		if (!hMutex)
			break;

		if (GetLastError() == ERROR_ALREADY_EXISTS)
			break;

		//
		// Create the File Mapping.
		//

		hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
			0, sizeof(DIHOOK_SHARED_DATA), DIHOOK_FILE_MAPPING_NAME);
		if (!hFileMapping || GetLastError() != ERROR_SUCCESS)
			break;

		SharedMemory = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DIHOOK_SHARED_DATA));
		if (!SharedMemory)
			break;

		//
		// Initialize the shared data.
		//

		InitializeSharedData((PDIHOOK_SHARED_DATA)SharedMemory);

		//
		// Create the heartbeat thread.
		//

		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CDInputKeyboardHook::ServerThread, (PVOID)this, 0, NULL);
		if (!hThread)
			break;

		Initialized = TRUE;
	} while (FALSE);

	if (!Initialized)
	{
		CloseHandle(hThread);

		UnmapViewOfFile(SharedMemory);
		CloseHandle(hFileMapping);
		CloseHandle(hMutex);

		return FALSE;
	}

	LOCK_DATA();

	m_SelfMutexHandle = hMutex;
	m_MappingHandle = hFileMapping;
	m_SharedData = (PDIHOOK_SHARED_DATA)SharedMemory;
	m_ThreadHandle = hThread;

	UNLOCK_DATA();

	return TRUE;
}

VOID CDInputKeyboardHook::Shutdown()
{
	__try
	{
		m_SharedData->Flags |= DIHOOK_FLAG_SHUTDOWN;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	LOCK_DATA();

	UnmapViewOfFile(m_SharedData);
	CloseHandle(m_MappingHandle);
	CloseHandle(m_SelfMutexHandle);

	// Wait for thread termination
	WaitForSingleObject(m_ThreadHandle, 1000);
	CloseHandle(m_ThreadHandle);

	UNLOCK_DATA();
}


BOOL CDInputKeyboardHook::SendUninstallHookMessage()
{
	BOOL bResult = TRUE;

	LOCK_DATA();
	bResult = SendUninstallHookMessageAssertLock();
	UNLOCK_DATA();

	return bResult;
}

BOOL CDInputKeyboardHook::SendUninstallHookMessageAssertLock()
{
	BOOL bInitialized = TRUE;

	if (!IS_DATA_LOCKED())
		__debugbreak();

	if (m_SharedData)
	{
		m_SharedData->Flags |= DIHOOK_FLAG_SHUTDOWN;
		Sleep(500);
	}
	else
	{
		bInitialized = FALSE;
	}

	return bInitialized;
}


BOOL CDInputKeyboardHook::SetKeyMapping(BYTE Mapping[256])
{
	BOOL bInitialized = TRUE;

	LOCK_DATA();

	if (m_SharedData)
		memcpy(m_SharedData->KeyRemapping, Mapping, 0x100);
	else
		bInitialized = FALSE;

	UNLOCK_DATA();

	return bInitialized;
}

BOOL CDInputKeyboardHook::SetKeyMapping(BYTE ScanCode, BYTE MapTo)
{
	BOOL bInitialized = TRUE;

	LOCK_DATA();

	if (m_SharedData)
		m_SharedData->KeyRemapping[ScanCode] = MapTo;
	else
		bInitialized = FALSE;

	UNLOCK_DATA();

	return bInitialized;
}

BOOL CDInputKeyboardHook::GetKeyMapping(BYTE Mapping[256])
{
	BOOL bInitialized = TRUE;

	LOCK_DATA();

	if (m_SharedData)
		memcpy(Mapping, m_SharedData->KeyRemapping, 0x100);
	else
		bInitialized = FALSE;

	UNLOCK_DATA();

	return bInitialized;
}

BYTE CDInputKeyboardHook::GetKeyMapping(BYTE ScanCode)
{
	BYTE MapTo = 0;

	LOCK_DATA();

	if (m_SharedData)
		MapTo = m_SharedData->KeyRemapping[ScanCode];

	UNLOCK_DATA();

	return MapTo;
}

BOOL CDInputKeyboardHook::DeleteKeyMapping()
{
	BOOL bInitialized = TRUE;

	LOCK_DATA();

	if (m_SharedData)
	{
		for (int i = 0; i < 256; i++)
			m_SharedData->KeyRemapping[i] = i;
	}
	else
		bInitialized = FALSE;

	UNLOCK_DATA();

	return bInitialized;
}

BOOL CDInputKeyboardHook::DeleteKeyMapping(BYTE ScanCode)
{
	return SetKeyMapping(ScanCode, ScanCode);
}

BOOL CDInputKeyboardHook::IsHookInstalled(DWORD ProcessId)
{
	WCHAR MutexName[100];
	HANDLE MutexHandle;

	wsprintf(MutexName, DIHOOK_MUTEX_NAME, ProcessId);
	MutexHandle = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MutexName);
	if (MutexHandle)
	{
		CloseHandle(MutexHandle);

		if (ProcessId != GetCurrentProcessId())
			return TRUE;
	}

	return FALSE;
}

BOOL CDInputKeyboardHook::InstallHook(DWORD ProcessId)
{
	WCHAR ModulePath[MAX_PATH];
	GetRemapperModulePathW(ModulePath, _countof(ModulePath));
	return InjectModuleW(ProcessId, ModulePath);
}

BOOL CDInputKeyboardHook::UninstallHook()
{
	return SendUninstallHookMessage();
}

BOOL CDInputKeyboardHook::UninstallHook(DWORD ProcessId)
{
	WCHAR ModulePath[MAX_PATH];
	HMODULE hModule;
	HANDLE ProcessHandle;
	HANDLE SnapshotHandle;
	MODULEENTRY32W Entry;
	DWORD_PTR Offset;
	BOOL Found;

	BOOL MessageSent = FALSE;

	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (!ProcessHandle)
		return FALSE;

	GetRemapperModulePathW(ModulePath, _countof(ModulePath));
	hModule = LoadLibraryExW(ModulePath, NULL, DONT_RESOLVE_DLL_REFERENCES);

	if (!hModule)
	{
		CloseHandle(ProcessHandle);
		return FALSE;
	}

	Offset = (DWORD_PTR)GetProcAddress(hModule, "DIHook_LocalData");
	FreeLibrary(hModule);

	if (!Offset)
	{
		CloseHandle(ProcessHandle);
		return FALSE;
	}

	Offset -= (DWORD_PTR)hModule;

	SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

	if (!SnapshotHandle)
	{
		CloseHandle(ProcessHandle);
		return FALSE;
	}

	Entry.dwSize = sizeof(Entry);
	Found = Module32FirstW(SnapshotHandle, &Entry);

	while (Found)
	{
		DWORD_PTR Address = (DWORD_PTR)Entry.hModule + Offset;

		if (!_wcsicmp(PathFindFileNameW(Entry.szModule), PathFindFileNameW(ModulePath)))
		{
			DIHOOK_LOCAL_DATA LocalData;
			DWORD BytesRead;
			DWORD BytesWritten;

			if (ReadProcessMemory(ProcessHandle, (LPCVOID)Address, &LocalData, sizeof(LocalData), &BytesRead) && 
				BytesRead == sizeof(LocalData) && 
				LocalData.SelfBase == Entry.hModule && 
				LocalData.SelfProcessId == Entry.th32ProcessID)
			{
				// Zero-fill write.
				ZeroMemory(&LocalData, sizeof(LocalData));
				MessageSent = WriteProcessMemory(ProcessHandle, (LPVOID)Address, &LocalData, sizeof(LocalData), &BytesWritten);
				break;
			}
		}

		Found = Module32NextW(SnapshotHandle, &Entry);
	}

	CloseHandle(SnapshotHandle);
	CloseHandle(ProcessHandle);

	return MessageSent;
}
