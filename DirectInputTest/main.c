
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

#define	DIRECTINPUT_VERSION			0x0800

#include <stdio.h>
#include <Windows.h>
#include <dinput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

typedef struct _DINPUT_KEY_INFO {
	PSZ Name;
	BYTE Value;
} DINPUT_KEY_INFO;

#define _KEYNAME_ENTRY(_x)	\
	{ "" #_x "", (_x) }

DINPUT_KEY_INFO DInput_KeyNames[] = {
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


int main()
{
	LPDIRECTINPUT8A pDInput8A = NULL;
	LPDIRECTINPUTDEVICE8A pDInputDeviceA = NULL;

	HRESULT Result;
	BYTE Packet[256];
	ULONG SequenceNumber;

	printf(":::::::::::::::: DirectInput8 Test ::::::::::::::::\n\n");

	Result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, &IID_IDirectInput8A, &pDInput8A, NULL);
	if (FAILED(Result))
	{
		printf("DirectInput8Create failed\n");
		return -1;
	}

	Result = pDInput8A->lpVtbl->CreateDevice(pDInput8A, &GUID_SysKeyboard, &pDInputDeviceA, NULL);
	if (FAILED(Result))
	{
		printf("CreateDevice failed\n");

		pDInput8A->lpVtbl->Release(pDInput8A);
		return -1;
	}

	if (FAILED(pDInputDeviceA->lpVtbl->SetDataFormat(pDInputDeviceA, &c_dfDIKeyboard)) ||
		FAILED(pDInputDeviceA->lpVtbl->SetCooperativeLevel(
			pDInputDeviceA, GetConsoleWindow(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		printf("SetDataFormat / SetCooperativeLevel failed\n");

		pDInputDeviceA->lpVtbl->Release(pDInputDeviceA);
		pDInput8A->lpVtbl->Release(pDInput8A);
		return -1;
	}

	ZeroMemory(Packet, sizeof(Packet));

	for (SequenceNumber = 0; ; )
	{
		INT Count = 0;

		do
		{
			Result = pDInputDeviceA->lpVtbl->Acquire(pDInputDeviceA);
		} while (Result == DIERR_INPUTLOST);

		if (FAILED(Result))
		{
			printf("Acquire failed, hr = 0x%08x\n", Result);
			break;
		}

		pDInputDeviceA->lpVtbl->GetDeviceState(pDInputDeviceA, sizeof(Packet), Packet);

		for (int i = 0; i < _countof(Packet); i++)
		{
			PSZ Name = "Unknown";

			if (!(Packet[i] & 0x80))
				continue;

			for (int j = 0; j < _countof(DInput_KeyNames); j++)
			{
				if (DInput_KeyNames[j].Value == i)
				{
					Name = DInput_KeyNames[j].Name;
					break;
				}
			}

			if (!Count)
				printf("%8d: ", SequenceNumber);

			printf("%s ", Name);
			Count++;
		}

		if (Count)
		{
			SequenceNumber++;
			printf("\n");
		}

		Sleep(10);
	}


	pDInputDeviceA->lpVtbl->Unacquire(pDInputDeviceA);
	pDInputDeviceA->lpVtbl->Release(pDInputDeviceA);
	pDInput8A->lpVtbl->Release(pDInput8A);

	return 0;
}

