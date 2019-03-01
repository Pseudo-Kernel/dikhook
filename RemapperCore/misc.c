
/*
* DIKHook, DirectInput8 Keyboard Input Remapper.
* https://bitbucket.org/Pseudo-Kernel/DIKHook
*
* The MIT License
*
* Copyright (c) 2019, Pseudo-Kernel
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
* IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <Windows.h>
#include <dinput.h>
#include <TlHelp32.h>

#include "global.h"
#include "misc.h"
#include "di8hook.h"


VOID
DTRACE(
	IN PSZ Format,
	...)
{
	CHAR Message[512];
	va_list Args;

	va_start(Args, Format);
	vsnprintf(Message, _countof(Message), Format, Args);
	va_end(Args);

	OutputDebugStringA(Message);
}

VOID
DIHook_SelfUnload(
	VOID)
{
	HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	DWORD ModuleRef = 1;

	if (SnapshotHandle)
	{
		MODULEENTRY32W Entry;
		BOOL Found;

		Entry.dwSize = sizeof(Entry);
		Found = Module32FirstW(SnapshotHandle, &Entry);
		while (Found)
		{
			if (Entry.hModule == DIHook_ModuleBase)
			{
				ModuleRef = Entry.GlblcntUsage;
				break;
			}

			Found = Module32NextW(SnapshotHandle, &Entry);
		}

		CloseHandle(SnapshotHandle);
	}

	DTRACE("Self unloading, Ref = %d\n", ModuleRef);

	while (ModuleRef-- > 1)
		FreeLibrary(DIHook_ModuleBase);

	FreeLibraryAndExitThread(DIHook_ModuleBase, 0);
}

DWORD
WINAPI
DIHook_ClientThread(
	IN PDIHOOK_CONTEXT Context)
{
	ULONG HeartbeatPrev = 0;
	DWORD TickPrev = 0;

	__try
	{
		HeartbeatPrev = Context->SharedData->Heartbeat;
		TickPrev = GetTickCount();

		for (;;)
		{
			ULONG Heartbeat = Context->SharedData->Heartbeat;
			ULONG HeartbeatDelta = Heartbeat - HeartbeatPrev;
			DWORD Tick = GetTickCount();
			DWORD TickDelta = Tick - TickPrev;

			if (!(Context->SharedData->Flags & DIHOOK_FLAG_HEARTBEAT_SUSPENDED))
			{
				if(!(Heartbeat % 10) || !HeartbeatDelta)
					DTRACE("HeartbeatDelta %d, TickDelta %d\n", HeartbeatDelta, TickDelta);

				if (!HeartbeatDelta)
				{
					if (TickDelta >= DIHOOK_HEARTBEAT_TIMEOUT)
					{
						// Heartbeat timed out.
						DTRACE("Heartbeat timed out\n");
						break;
					}
				}
				else
				{
					HeartbeatPrev = Heartbeat;
					TickPrev = Tick;
				}
			}

			if ((Context->SharedData->Flags & DIHOOK_FLAG_SHUTDOWN)
				// Check the local data.
				|| DIHook_LocalData.SelfBase != DIHook_ModuleBase
				|| DIHook_LocalData.SelfProcessId != GetCurrentProcessId())
			{
				// Server wants to close all client instances.
				DTRACE("Received shutdown request from server\n");
				break;
			}

			Sleep(500);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// Exception!
	}

	// Do the cleanup.
	DIHook_SelfUnload();

	return 0;
}

BOOL
WINAPI
DIHook_Initialize(
	OUT PDIHOOK_CONTEXT Context, 
	OUT PDIHOOK_LOCAL_DATA LocalData)
{
	WCHAR MutexName[100];
	HANDLE hMutex = NULL;
	HANDLE hFileMapping = NULL;
	PVOID SharedMemory = NULL;
	HANDLE hThread = NULL;
	BOOLEAN Initialized = FALSE;

	ZeroMemory(Context, sizeof(*Context));

	do
	{
		//
		// Create Our Mutex.
		//

		wsprintf(MutexName, DIHOOK_MUTEX_NAME, GetCurrentProcessId());
		hMutex = CreateMutex(NULL, TRUE, MutexName);

		if (!hMutex)
		{
			DTRACE("Failed to create mutex\n");
			break;
		}

		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			DTRACE("Mutex already exists\n");
			break;
		}

		//
		// Open the File Mapping.
		//

		hFileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, DIHOOK_FILE_MAPPING_NAME);
		if (!hFileMapping)
		{
			DTRACE("Failed to open file mapping\n");
			break;
		}

		SharedMemory = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, sizeof(DIHOOK_SHARED_DATA));
		if (!SharedMemory)
		{
			DTRACE("Failed to map view of file\n");
			break;
		}

		//
		// Initialize the local data.
		//

		LocalData->SelfBase = DIHook_ModuleBase;
		LocalData->SelfProcessId = GetCurrentProcessId();

		//
		// Create the Notification Thread.
		//

		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DIHook_ClientThread,
			&DIHook_DllContext, 0, NULL);
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

		DTRACE("Failed to initialize\n");

		return FALSE;
	}

	Context->SelfMutexHandle = hMutex;
	Context->MappingHandle = hFileMapping;
	Context->SharedData = SharedMemory;
	Context->ThreadHandle = hThread;

	DTRACE("Initialized\n");

	return TRUE;
}

VOID
WINAPI
DIHook_Shutdown(
	IN PDIHOOK_CONTEXT Context, 
	IN PDIHOOK_LOCAL_DATA LocalData)
{
	DTRACE("Initializing shutdown...\n");

	// Unmap shared memory first to make our thread stop (by exception)
	UnmapViewOfFile(Context->SharedData);
	CloseHandle(Context->MappingHandle);
	CloseHandle(Context->SelfMutexHandle);

	LocalData->SelfBase = NULL;
	LocalData->SelfProcessId = 0;

	DTRACE("Waiting for thread termination...\n");

	// Wait for thread termination
	WaitForSingleObject(Context->ThreadHandle, 1000);

	DTRACE("Closing the handle to pay respect\n");
	CloseHandle(Context->ThreadHandle);

	ZeroMemory(Context, sizeof(*Context));
}

BYTE
DIHook_GetKeyRemappingUnsafe(
	IN BYTE ScanCode)
{
	return DIHook_DllContext.SharedData->KeyRemapping[ScanCode];
}

BYTE
DIHook_GetKeyRemapping(
	IN BYTE ScanCode)
{
	__try
	{
		return DIHook_DllContext.SharedData->KeyRemapping[ScanCode];
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	return ScanCode;
}

VOID
DIHook_RemapKeyData(
	IN OUT BYTE Packet[256])
{
	__try
	{
		BYTE NewPacket[DINPUT_KEYSTATE_PACKET_SIZE];

		ZeroMemory(NewPacket, sizeof(NewPacket));

		//
		// Remap the key input state.
		//

		for (INT i = 0; i < DINPUT_KEYSTATE_PACKET_SIZE; i++)
		{
			BYTE NewMapping = DIHook_GetKeyRemappingUnsafe(i);
			NewPacket[NewMapping] |= Packet[i];
		}

		NewPacket[0] = 0; // Null mapping

		memcpy(Packet, NewPacket, sizeof(NewPacket));
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

