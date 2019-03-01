
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

typedef
HRESULT
(APIENTRY *IDIRECTINPUTDEVICE8A_GETDEVICEDATA)(
	IN LPDIRECTINPUTDEVICE8A This,
	IN DWORD cbObjectData,
	IN LPDIDEVICEOBJECTDATA rgdod,
	OUT LPDWORD pdwInOut,
	IN DWORD dwFlags);

typedef
HRESULT
(APIENTRY *IDIRECTINPUTDEVICE8A_GETDEVICESTATE)(
	IN LPDIRECTINPUTDEVICE8A This,
	IN DWORD cbData,
	OUT LPVOID lpvData);

typedef
HRESULT
(APIENTRY *IDIRECTINPUTDEVICE8W_GETDEVICEDATA)(
	IN LPDIRECTINPUTDEVICE8W This,
	IN DWORD cbObjectData,
	IN LPDIDEVICEOBJECTDATA rgdod,
	OUT LPDWORD pdwInOut,
	IN DWORD dwFlags);

typedef
HRESULT
(APIENTRY *IDIRECTINPUTDEVICE8W_GETDEVICESTATE)(
	IN LPDIRECTINPUTDEVICE8W This,
	IN DWORD cbData,
	OUT LPVOID lpvData);


extern volatile IDirectInputDevice8AVtbl *DIHook_IDirectInputDevice8A_Vtbl;
extern volatile IDirectInputDevice8WVtbl *DIHook_IDirectInputDevice8W_Vtbl;

extern volatile IDIRECTINPUTDEVICE8A_GETDEVICEDATA DIHook_IDirectInputDevice8A_GetDeviceData_Previous;
extern volatile IDIRECTINPUTDEVICE8A_GETDEVICESTATE DIHook_IDirectInputDevice8A_GetDeviceState_Previous;
extern volatile IDIRECTINPUTDEVICE8W_GETDEVICEDATA DIHook_IDirectInputDevice8W_GetDeviceData_Previous;
extern volatile IDIRECTINPUTDEVICE8W_GETDEVICESTATE DIHook_IDirectInputDevice8W_GetDeviceState_Previous;


HRESULT
APIENTRY
DIHook_IDirectInputDevice8A_GetDeviceData(
	IN LPDIRECTINPUTDEVICE8A This,
	IN DWORD cbObjectData,
	IN LPDIDEVICEOBJECTDATA rgdod,
	OUT LPDWORD pdwInOut,
	IN DWORD dwFlags);

HRESULT
APIENTRY
DIHook_IDirectInputDevice8W_GetDeviceData(
	IN LPDIRECTINPUTDEVICE8W This,
	IN DWORD cbObjectData,
	IN LPDIDEVICEOBJECTDATA rgdod,
	OUT LPDWORD pdwInOut,
	IN DWORD dwFlags);

HRESULT
APIENTRY
DIHook_IDirectInputDevice8A_GetDeviceState(
	IN LPDIRECTINPUTDEVICE8A This,
	IN DWORD cbData,
	OUT LPVOID lpvData);

HRESULT
APIENTRY
DIHook_IDirectInputDevice8W_GetDeviceState(
	IN LPDIRECTINPUTDEVICE8W This,
	IN DWORD cbData,
	OUT LPVOID lpvData);


BOOL
InstallHook(
	VOID);

BOOL
UninstallHook(
	VOID);

