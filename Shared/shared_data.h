
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

#pragma pack(push, 1)

#define	DINPUT_KEYSTATE_PACKET_SIZE				256

typedef struct _DIHOOK_SHARED_DATA {
	BYTE KeyRemapping[DINPUT_KEYSTATE_PACKET_SIZE];		// Our key remapping data.
	ULONG Flags;					// Global mode flags.
	ULONG Heartbeat;				// Updated by the server process.
} DIHOOK_SHARED_DATA, *PDIHOOK_SHARED_DATA;

#define	DIHOOK_FLAG_SHUTDOWN				0x00000001
#define	DIHOOK_FLAG_HEARTBEAT_SUSPENDED		0x00000002

#define	DIHOOK_MUTEX_NAME					L"__KEY_REMAPPER_%X__"
#define	DIHOOK_FILE_MAPPING_NAME			L"__KEY_REMAPPER_SMAP_OBJECT__"
#define	DIHOOK_HEARTBEAT_TIMEOUT			2000

typedef struct _DIHOOK_LOCAL_DATA {
	HMODULE SelfBase;
	ULONG SelfProcessId;
} DIHOOK_LOCAL_DATA, *PDIHOOK_LOCAL_DATA;

#pragma pack(pop)


typedef struct _DIHOOK_CONTEXT {
	struct _DIHOOK_SHARED_DATA *SharedData;
	HANDLE SelfMutexHandle;
	HANDLE MappingHandle;
	HANDLE ThreadHandle;
} DIHOOK_CONTEXT, *PDIHOOK_CONTEXT;

#define	DIHOOK_REMAPPER_CORE_DLL_NAME				L"RemapperCore.dll"

