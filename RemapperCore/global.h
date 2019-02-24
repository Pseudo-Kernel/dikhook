
#pragma once

#include "../shared/shared_data.h"

extern HMODULE DIHook_ModuleBase;
extern BOOL DIHook_HookInstalled;
extern DIHOOK_CONTEXT DIHook_DllContext;
extern __declspec(align(16)) DIHOOK_LOCAL_DATA DIHook_LocalData;

