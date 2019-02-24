#include <stdio.h>
#include <Windows.h>
#include <dinput.h>

#include "global.h"
#include "di8hook.h"
#include "misc.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

volatile IDirectInputDevice8AVtbl *DIHook_IDirectInputDevice8A_Vtbl;
volatile IDirectInputDevice8WVtbl *DIHook_IDirectInputDevice8W_Vtbl;

volatile IDIRECTINPUTDEVICE8A_GETDEVICEDATA DIHook_IDirectInputDevice8A_GetDeviceData_Previous;
volatile IDIRECTINPUTDEVICE8A_GETDEVICESTATE DIHook_IDirectInputDevice8A_GetDeviceState_Previous;
volatile IDIRECTINPUTDEVICE8W_GETDEVICEDATA DIHook_IDirectInputDevice8W_GetDeviceData_Previous;
volatile IDIRECTINPUTDEVICE8W_GETDEVICESTATE DIHook_IDirectInputDevice8W_GetDeviceState_Previous;



HRESULT
APIENTRY
DIHook_IDirectInputDevice8A_GetDeviceData(
	IN LPDIRECTINPUTDEVICE8A This,
	IN DWORD cbObjectData,
	IN LPDIDEVICEOBJECTDATA rgdod,
	OUT LPDWORD pdwInOut,
	IN DWORD dwFlags)
{
	HRESULT Result = DIHook_IDirectInputDevice8A_GetDeviceData_Previous(
		This, cbObjectData, rgdod, pdwInOut, dwFlags);

	DTRACE("DIHook_IDirectInputDevice8A_GetDeviceData\n");

	if (rgdod && SUCCEEDED(Result))
	{
		DIDEVICEINSTANCEA DeviceInstance;

		ZeroMemory(&DeviceInstance, sizeof(DeviceInstance));
		DeviceInstance.dwSize = sizeof(DeviceInstance);

		if (SUCCEEDED(This->lpVtbl->GetDeviceInfo(This, &DeviceInstance)))
		{
			if (((DeviceInstance.dwDevType & 0xff00) >> 0x08) == DI8DEVTYPE_KEYBOARD)
			{
				DWORD Count = *pdwInOut;
				for (DWORD i = 0; i < Count; i++)
				{
					LPDIDEVICEOBJECTDATA pData =
						(LPDIDEVICEOBJECTDATA)((PBYTE)rgdod + cbObjectData * i);
					pData->dwOfs = DIHook_GetKeyRemapping((BYTE)pData->dwOfs);
				}
			}
		}
	}

	return Result;
}

HRESULT
APIENTRY
DIHook_IDirectInputDevice8W_GetDeviceData(
	IN LPDIRECTINPUTDEVICE8W This,
	IN DWORD cbObjectData,
	IN LPDIDEVICEOBJECTDATA rgdod,
	OUT LPDWORD pdwInOut,
	IN DWORD dwFlags)
{
	HRESULT Result = DIHook_IDirectInputDevice8W_GetDeviceData_Previous(
		This, cbObjectData, rgdod, pdwInOut, dwFlags);

	DTRACE("DIHook_IDirectInputDevice8W_GetDeviceData\n");

	if (rgdod && SUCCEEDED(Result))
	{
		DIDEVICEINSTANCEW DeviceInstance;

		ZeroMemory(&DeviceInstance, sizeof(DeviceInstance));
		DeviceInstance.dwSize = sizeof(DeviceInstance);

		if (SUCCEEDED(This->lpVtbl->GetDeviceInfo(This, &DeviceInstance)))
		{
			if (((DeviceInstance.dwDevType & 0xff00) >> 0x08) == DI8DEVTYPE_KEYBOARD)
			{
				DWORD Count = *pdwInOut;
				for (DWORD i = 0; i < Count; i++)
				{
					LPDIDEVICEOBJECTDATA pData =
						(LPDIDEVICEOBJECTDATA)((PBYTE)rgdod + cbObjectData * i);
					pData->dwOfs = DIHook_GetKeyRemapping((BYTE)pData->dwOfs);
				}
			}
		}
	}

	return Result;
}

HRESULT
APIENTRY
DIHook_IDirectInputDevice8A_GetDeviceState(
	IN LPDIRECTINPUTDEVICE8A This,
	IN DWORD cbData,
	OUT LPVOID lpvData)
{
	HRESULT Result = DIHook_IDirectInputDevice8A_GetDeviceState_Previous(This, cbData, lpvData);

//	DTRACE("DIHook_IDirectInputDevice8A_GetDeviceState\n");

	if (SUCCEEDED(Result))
	{
		switch (cbData)
		{
		case sizeof(BYTE[256]) : // IDirectInputDevice8::SetDataFormat(&c_dfDIKeyboard)
			DIHook_RemapKeyData(lpvData);
			break;

		case sizeof(DIMOUSESTATE): // IDirectInputDevice8::SetDataFormat(&c_dfDIMouse)
		case sizeof(DIMOUSESTATE2): // IDirectInputDevice8::SetDataFormat(&c_dfDIMouse2)
		case sizeof(DIJOYSTATE): // IDirectInputDevice8::SetDataFormat(&c_dfDIJoystick)
		case sizeof(DIJOYSTATE2): // IDirectInputDevice8::SetDataFormat(&c_dfDIJoystick2)
		default:
			break;
		}
	}

	return Result;
}

HRESULT
APIENTRY
DIHook_IDirectInputDevice8W_GetDeviceState(
	IN LPDIRECTINPUTDEVICE8W This,
	IN DWORD cbData,
	OUT LPVOID lpvData)
{
	HRESULT Result = DIHook_IDirectInputDevice8W_GetDeviceState_Previous(This, cbData, lpvData);

//	DTRACE("DIHook_IDirectInputDevice8W_GetDeviceState\n");

	if (SUCCEEDED(Result))
	{
		if(cbData == sizeof(BYTE[DINPUT_KEYSTATE_PACKET_SIZE]))
			DIHook_RemapKeyData(lpvData);
	}

	return Result;
}


BOOL
InstallHook(
	VOID)
{
	LPDIRECTINPUT8A pDInput8A = NULL;
	LPDIRECTINPUT8W pDInput8W = NULL;
	LPDIRECTINPUTDEVICE8A pDInputDeviceA = NULL;
	LPDIRECTINPUTDEVICE8W pDInputDeviceW = NULL;

	IDirectInputDevice8AVtbl *pVtblA;
	IDirectInputDevice8WVtbl *pVtblW;

	HMODULE hModule = GetModuleHandle(NULL);
	BOOL Succeeded = FALSE;

	if (IsDebuggerPresent())
		__debugbreak();

	do
	{
		HRESULT Result;
		DWORD PrevProtect;

		Result = DirectInput8Create(hModule, DIRECTINPUT_VERSION, &IID_IDirectInput8A, &pDInput8A, NULL);
		if (FAILED(Result))
			break;

		Result = DirectInput8Create(hModule, DIRECTINPUT_VERSION, &IID_IDirectInput8W, &pDInput8W, NULL);
		if (FAILED(Result))
			break;

		Result = pDInput8A->lpVtbl->CreateDevice(pDInput8A, &GUID_SysKeyboard, &pDInputDeviceA, NULL);
		if (FAILED(Result))
			break;

		Result = pDInput8W->lpVtbl->CreateDevice(pDInput8W, &GUID_SysKeyboard, &pDInputDeviceW, NULL);
		if (FAILED(Result))
			break;

		// Get the address of vftable and functions.

		pVtblA = pDInputDeviceA->lpVtbl;
		pVtblW = pDInputDeviceW->lpVtbl;

		DIHook_IDirectInputDevice8A_Vtbl = pVtblA;
		DIHook_IDirectInputDevice8W_Vtbl = pVtblW;
		DIHook_IDirectInputDevice8A_GetDeviceData_Previous = pVtblA->GetDeviceData;
		DIHook_IDirectInputDevice8A_GetDeviceState_Previous = pVtblA->GetDeviceState;
		DIHook_IDirectInputDevice8W_GetDeviceData_Previous = pVtblW->GetDeviceData;
		DIHook_IDirectInputDevice8W_GetDeviceState_Previous = pVtblW->GetDeviceState;


		// Modify the vftable.

		VirtualProtect(pVtblA, sizeof(*pVtblA), PAGE_READWRITE, &PrevProtect);
		VirtualProtect(pVtblW, sizeof(*pVtblW), PAGE_READWRITE, &PrevProtect);

		DTRACE("GetDeviceDataA = 0x%p\n", pVtblA->GetDeviceData);
		DTRACE("GetDeviceDataW = 0x%p\n", pVtblW->GetDeviceData);
		DTRACE("GetDeviceStateA = 0x%p\n", pVtblA->GetDeviceState);
		DTRACE("GetDeviceStateW = 0x%p\n", pVtblW->GetDeviceState);

		pVtblA->GetDeviceData = DIHook_IDirectInputDevice8A_GetDeviceData;
		pVtblA->GetDeviceState = DIHook_IDirectInputDevice8A_GetDeviceState;
		pVtblW->GetDeviceData = DIHook_IDirectInputDevice8W_GetDeviceData;
		pVtblW->GetDeviceState = DIHook_IDirectInputDevice8W_GetDeviceState;

		DIHook_HookInstalled = TRUE;
		Succeeded = TRUE;
	} while (FALSE);

	if (pDInput8A) pDInput8A->lpVtbl->Release(pDInput8A);
	if (pDInput8W) pDInput8W->lpVtbl->Release(pDInput8W);
	if (pDInputDeviceA) pDInputDeviceA->lpVtbl->Release(pDInputDeviceA);
	if (pDInputDeviceW) pDInputDeviceW->lpVtbl->Release(pDInputDeviceW);

	if (Succeeded)
		DTRACE("Hook installed\n");
	else
		DTRACE("Failed to install hook\n");

	return Succeeded;
}

BOOL
UninstallHook(
	VOID)
{
	if (!DIHook_HookInstalled)
		return FALSE;

	// Restore the vftable.
	DIHook_IDirectInputDevice8A_Vtbl->GetDeviceData = DIHook_IDirectInputDevice8A_GetDeviceData_Previous;
	DIHook_IDirectInputDevice8A_Vtbl->GetDeviceState = DIHook_IDirectInputDevice8A_GetDeviceState_Previous;
	DIHook_IDirectInputDevice8W_Vtbl->GetDeviceData = DIHook_IDirectInputDevice8W_GetDeviceData_Previous;
	DIHook_IDirectInputDevice8W_Vtbl->GetDeviceState = DIHook_IDirectInputDevice8W_GetDeviceState_Previous;

	DIHook_HookInstalled = FALSE;

	DTRACE("Hook uninstalled\n");

	return TRUE;
}

