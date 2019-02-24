
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

