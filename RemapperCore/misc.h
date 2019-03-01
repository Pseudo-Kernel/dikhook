
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


