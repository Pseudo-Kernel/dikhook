
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
#include "di8hook.h"
#include "misc.h"


HMODULE DIHook_ModuleBase;
BOOL DIHook_HookInstalled = FALSE;
DIHOOK_CONTEXT DIHook_DllContext;
__declspec(align(16)) DIHOOK_LOCAL_DATA DIHook_LocalData;


BOOL
APIENTRY
DllMain(
	IN HMODULE hModule,
	IN DWORD dwReason,
	IN PVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DIHook_ModuleBase = hModule;
		if (!DIHook_Initialize(&DIHook_DllContext, &DIHook_LocalData))
			return FALSE;
		InstallHook();
		break;

	case DLL_PROCESS_DETACH:
		UninstallHook();
		DIHook_Shutdown(&DIHook_DllContext, &DIHook_LocalData);
		break;
	}

	return TRUE;
}

