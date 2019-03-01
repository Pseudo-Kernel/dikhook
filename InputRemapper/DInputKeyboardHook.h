
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
