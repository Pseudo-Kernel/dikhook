
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
#include "DInputKeyboard.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

typedef struct _DINPUT_KEY_INFO {
	PSZ Name;
	BYTE Value;
} DINPUT_KEY_INFO;

#define _KEYNAME_ENTRY(_x)	\
	{ "" #_x "", (_x) }

DINPUT_KEY_INFO __DInput8_KeyNames[] = {
	{ "< Null-Mapping >", 0 }, 
	_KEYNAME_ENTRY(DIK_ESCAPE),
	_KEYNAME_ENTRY(DIK_1),
	_KEYNAME_ENTRY(DIK_2),
	_KEYNAME_ENTRY(DIK_3),
	_KEYNAME_ENTRY(DIK_4),
	_KEYNAME_ENTRY(DIK_5),
	_KEYNAME_ENTRY(DIK_6),
	_KEYNAME_ENTRY(DIK_7),
	_KEYNAME_ENTRY(DIK_8),
	_KEYNAME_ENTRY(DIK_9),
	_KEYNAME_ENTRY(DIK_0),
	_KEYNAME_ENTRY(DIK_MINUS),
	_KEYNAME_ENTRY(DIK_EQUALS),
	_KEYNAME_ENTRY(DIK_BACK),
	_KEYNAME_ENTRY(DIK_TAB),
	_KEYNAME_ENTRY(DIK_Q),
	_KEYNAME_ENTRY(DIK_W),
	_KEYNAME_ENTRY(DIK_E),
	_KEYNAME_ENTRY(DIK_R),
	_KEYNAME_ENTRY(DIK_T),
	_KEYNAME_ENTRY(DIK_Y),
	_KEYNAME_ENTRY(DIK_U),
	_KEYNAME_ENTRY(DIK_I),
	_KEYNAME_ENTRY(DIK_O),
	_KEYNAME_ENTRY(DIK_P),
	_KEYNAME_ENTRY(DIK_LBRACKET),
	_KEYNAME_ENTRY(DIK_RBRACKET),
	_KEYNAME_ENTRY(DIK_RETURN),
	_KEYNAME_ENTRY(DIK_LCONTROL),
	_KEYNAME_ENTRY(DIK_A),
	_KEYNAME_ENTRY(DIK_S),
	_KEYNAME_ENTRY(DIK_D),
	_KEYNAME_ENTRY(DIK_F),
	_KEYNAME_ENTRY(DIK_G),
	_KEYNAME_ENTRY(DIK_H),
	_KEYNAME_ENTRY(DIK_J),
	_KEYNAME_ENTRY(DIK_K),
	_KEYNAME_ENTRY(DIK_L),
	_KEYNAME_ENTRY(DIK_SEMICOLON),
	_KEYNAME_ENTRY(DIK_APOSTROPHE),
	_KEYNAME_ENTRY(DIK_GRAVE),
	_KEYNAME_ENTRY(DIK_LSHIFT),
	_KEYNAME_ENTRY(DIK_BACKSLASH),
	_KEYNAME_ENTRY(DIK_Z),
	_KEYNAME_ENTRY(DIK_X),
	_KEYNAME_ENTRY(DIK_C),
	_KEYNAME_ENTRY(DIK_V),
	_KEYNAME_ENTRY(DIK_B),
	_KEYNAME_ENTRY(DIK_N),
	_KEYNAME_ENTRY(DIK_M),
	_KEYNAME_ENTRY(DIK_COMMA),
	_KEYNAME_ENTRY(DIK_PERIOD),
	_KEYNAME_ENTRY(DIK_SLASH),
	_KEYNAME_ENTRY(DIK_RSHIFT),
	_KEYNAME_ENTRY(DIK_MULTIPLY),
	_KEYNAME_ENTRY(DIK_LMENU),
	_KEYNAME_ENTRY(DIK_SPACE),
	_KEYNAME_ENTRY(DIK_CAPITAL),
	_KEYNAME_ENTRY(DIK_F1),
	_KEYNAME_ENTRY(DIK_F2),
	_KEYNAME_ENTRY(DIK_F3),
	_KEYNAME_ENTRY(DIK_F4),
	_KEYNAME_ENTRY(DIK_F5),
	_KEYNAME_ENTRY(DIK_F6),
	_KEYNAME_ENTRY(DIK_F7),
	_KEYNAME_ENTRY(DIK_F8),
	_KEYNAME_ENTRY(DIK_F9),
	_KEYNAME_ENTRY(DIK_F10),
	_KEYNAME_ENTRY(DIK_NUMLOCK),
	_KEYNAME_ENTRY(DIK_SCROLL),
	_KEYNAME_ENTRY(DIK_NUMPAD7),
	_KEYNAME_ENTRY(DIK_NUMPAD8),
	_KEYNAME_ENTRY(DIK_NUMPAD9),
	_KEYNAME_ENTRY(DIK_SUBTRACT),
	_KEYNAME_ENTRY(DIK_NUMPAD4),
	_KEYNAME_ENTRY(DIK_NUMPAD5),
	_KEYNAME_ENTRY(DIK_NUMPAD6),
	_KEYNAME_ENTRY(DIK_ADD),
	_KEYNAME_ENTRY(DIK_NUMPAD1),
	_KEYNAME_ENTRY(DIK_NUMPAD2),
	_KEYNAME_ENTRY(DIK_NUMPAD3),
	_KEYNAME_ENTRY(DIK_NUMPAD0),
	_KEYNAME_ENTRY(DIK_DECIMAL),
	_KEYNAME_ENTRY(DIK_OEM_102),
	_KEYNAME_ENTRY(DIK_F11),
	_KEYNAME_ENTRY(DIK_F12),
	_KEYNAME_ENTRY(DIK_F13),
	_KEYNAME_ENTRY(DIK_F14),
	_KEYNAME_ENTRY(DIK_F15),
	_KEYNAME_ENTRY(DIK_KANA),
	_KEYNAME_ENTRY(DIK_ABNT_C1),
	_KEYNAME_ENTRY(DIK_CONVERT),
	_KEYNAME_ENTRY(DIK_NOCONVERT),
	_KEYNAME_ENTRY(DIK_YEN),
	_KEYNAME_ENTRY(DIK_ABNT_C2),
	_KEYNAME_ENTRY(DIK_NUMPADEQUALS),
	_KEYNAME_ENTRY(DIK_PREVTRACK),
	_KEYNAME_ENTRY(DIK_AT),
	_KEYNAME_ENTRY(DIK_COLON),
	_KEYNAME_ENTRY(DIK_UNDERLINE),
	_KEYNAME_ENTRY(DIK_KANJI),
	_KEYNAME_ENTRY(DIK_STOP),
	_KEYNAME_ENTRY(DIK_AX),
	_KEYNAME_ENTRY(DIK_UNLABELED),
	_KEYNAME_ENTRY(DIK_NEXTTRACK),
	_KEYNAME_ENTRY(DIK_NUMPADENTER),
	_KEYNAME_ENTRY(DIK_RCONTROL),
	_KEYNAME_ENTRY(DIK_MUTE),
	_KEYNAME_ENTRY(DIK_CALCULATOR),
	_KEYNAME_ENTRY(DIK_PLAYPAUSE),
	_KEYNAME_ENTRY(DIK_MEDIASTOP),
	_KEYNAME_ENTRY(DIK_VOLUMEDOWN),
	_KEYNAME_ENTRY(DIK_VOLUMEUP),
	_KEYNAME_ENTRY(DIK_WEBHOME),
	_KEYNAME_ENTRY(DIK_NUMPADCOMMA),
	_KEYNAME_ENTRY(DIK_DIVIDE),
	_KEYNAME_ENTRY(DIK_SYSRQ),
	_KEYNAME_ENTRY(DIK_RMENU),
	_KEYNAME_ENTRY(DIK_PAUSE),
	_KEYNAME_ENTRY(DIK_HOME),
	_KEYNAME_ENTRY(DIK_UP),
	_KEYNAME_ENTRY(DIK_PRIOR),
	_KEYNAME_ENTRY(DIK_LEFT),
	_KEYNAME_ENTRY(DIK_RIGHT),
	_KEYNAME_ENTRY(DIK_END),
	_KEYNAME_ENTRY(DIK_DOWN),
	_KEYNAME_ENTRY(DIK_NEXT),
	_KEYNAME_ENTRY(DIK_INSERT),
	_KEYNAME_ENTRY(DIK_DELETE),
	_KEYNAME_ENTRY(DIK_LWIN),
	_KEYNAME_ENTRY(DIK_RWIN),
	_KEYNAME_ENTRY(DIK_APPS),
	_KEYNAME_ENTRY(DIK_POWER),
	_KEYNAME_ENTRY(DIK_SLEEP),
	_KEYNAME_ENTRY(DIK_WAKE),
	_KEYNAME_ENTRY(DIK_WEBSEARCH),
	_KEYNAME_ENTRY(DIK_WEBFAVORITES),
	_KEYNAME_ENTRY(DIK_WEBREFRESH),
	_KEYNAME_ENTRY(DIK_WEBSTOP),
	_KEYNAME_ENTRY(DIK_WEBFORWARD),
	_KEYNAME_ENTRY(DIK_WEBBACK),
	_KEYNAME_ENTRY(DIK_MYCOMPUTER),
	_KEYNAME_ENTRY(DIK_MAIL),
	_KEYNAME_ENTRY(DIK_MEDIASELECT),
};

CDInputKeyboard::CDInputKeyboard() : m_pDInput8(NULL), m_pDInputDevice(NULL)
{

}

CDInputKeyboard::~CDInputKeyboard()
{
	FreeInstance();
}

BOOL CDInputKeyboard::CreateInstance(HMODULE hInstance, HWND hWnd, BOOL bNonExclusive, BOOL bAllowBackground)
{
	LPDIRECTINPUT8 pDInput8 = nullptr;
	LPDIRECTINPUTDEVICE8 pDInputDevice = nullptr;
	ULONG uFlags = 0;
	HRESULT Result;

	if (!hInstance)
		hInstance = GetModuleHandle(nullptr);

	Result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *)&pDInput8, nullptr);
	if (FAILED(Result))
		return FALSE;

	Result = pDInput8->CreateDevice(GUID_SysKeyboard, &pDInputDevice, nullptr);
	if (FAILED(Result))
	{
		pDInput8->Release();
		return FALSE;
	}

	Result = pDInputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(Result))
	{
		pDInputDevice->Release();
		pDInput8->Release();
		return FALSE;
	}

	if (bNonExclusive)
		uFlags |= DISCL_NONEXCLUSIVE;
	else
		uFlags |= DISCL_EXCLUSIVE;

	if (bAllowBackground)
		uFlags |= DISCL_BACKGROUND;
	else
		uFlags |= DISCL_FOREGROUND;

	Result = pDInputDevice->SetCooperativeLevel(hWnd, uFlags);
	if (FAILED(Result))
	{
		pDInputDevice->Release();
		pDInput8->Release();
		return FALSE;
	}

	m_pDInput8 = pDInput8;
	m_pDInputDevice = pDInputDevice;

	return TRUE;
}

VOID CDInputKeyboard::FreeInstance()
{
	if (m_pDInputDevice)
	{
		m_pDInputDevice->Unacquire();
		m_pDInputDevice->Release();
		m_pDInputDevice = nullptr;
	}

	if (m_pDInput8)
	{
		m_pDInput8->Release();
		m_pDInput8 = nullptr;
	}
}

CHAR *CDInputKeyboard::GetKeyString(BYTE ScanCode)
{
	for (DINPUT_KEY_INFO &it : __DInput8_KeyNames)
		if (it.Value == ScanCode)
			return it.Name;

	return (CHAR *)("Unknown");
}

BYTE CDInputKeyboard::GetKeyScanCode(CHAR *KeyString)
{
	for (DINPUT_KEY_INFO &it : __DInput8_KeyNames)
		if (!strcmp(it.Name, KeyString))
			return it.Value;

	return 0;
}

BOOL CDInputKeyboard::GetKeyState(BYTE Packet[256])
{
	HRESULT Result;

	if (!m_pDInputDevice)
		return FALSE;

	do
	{
		Result = m_pDInputDevice->Acquire();
	} while (Result == DIERR_INPUTLOST);

	if (FAILED(Result))
		return FALSE;

	Result = m_pDInputDevice->GetDeviceState(0x100, Packet);
	if (FAILED(Result))
		return FALSE;

	return TRUE;
}

