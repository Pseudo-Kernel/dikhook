#pragma once

#pragma unmanaged

class CDInputKeyboardHook {
public:
	CDInputKeyboardHook();
	~CDInputKeyboardHook();

	BOOL Initialize();
	VOID Shutdown();

	BOOL SetKeyMapping(BYTE Mapping[256]);
	BOOL SetKeyMapping(BYTE ScanCode, BYTE MapTo);
	BOOL GetKeyMapping(BYTE Mapping[256]);
	BYTE GetKeyMapping(BYTE ScanCode);
	BOOL DeleteKeyMapping();
	BOOL DeleteKeyMapping(BYTE ScanCode);

	BOOL IsHookInstalled(DWORD ProcessId);
	BOOL InstallHook(DWORD ProcessId);
	BOOL UninstallHook();
	BOOL UninstallHook(DWORD ProcessId);

private:
	static DWORD WINAPI ServerThread(IN CDInputKeyboardHook *This);
	VOID InitializeSharedData(struct _DIHOOK_SHARED_DATA *SharedData);

	BOOL SendUninstallHookMessage();
	BOOL SendUninstallHookMessageAssertLock();

private:
	ULONG m_Lock;
	struct _DIHOOK_SHARED_DATA *m_SharedData;
	HANDLE m_SelfMutexHandle;
	HANDLE m_MappingHandle;
	HANDLE m_ThreadHandle;
};

#pragma managed
