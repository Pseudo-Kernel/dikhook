
#pragma once

VOID
DTRACE(
	IN PSZ Format,
	...);

VOID
DIHook_SelfUnload(
	VOID);

DWORD
WINAPI
DIHook_ClientThread(
	IN PDIHOOK_CONTEXT Context);

BOOL
WINAPI
DIHook_Initialize(
	OUT PDIHOOK_CONTEXT Context,
	OUT PDIHOOK_LOCAL_DATA LocalData);

VOID
WINAPI
DIHook_Shutdown(
	IN PDIHOOK_CONTEXT Context,
	IN PDIHOOK_LOCAL_DATA LocalData);

BYTE
DIHook_GetKeyRemappingUnsafe(
	IN BYTE ScanCode);

BYTE
DIHook_GetKeyRemapping(
	IN BYTE ScanCode);

VOID
DIHook_RemapKeyData(
	IN OUT BYTE Packet[256]);


