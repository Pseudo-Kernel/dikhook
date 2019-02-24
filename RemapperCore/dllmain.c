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

