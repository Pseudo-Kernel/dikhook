
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

#pragma unmanaged

class CDInputKeyboard {

public:
	CDInputKeyboard();
	~CDInputKeyboard();

	BOOL CreateInstance(HMODULE hInstance, HWND hWnd, BOOL bNonExclusive, BOOL bAllowBackground);
	VOID FreeInstance();
	CHAR *GetKeyString(BYTE ScanCode);
	BYTE GetKeyScanCode(CHAR *KeyString);
	BOOL GetKeyState(BYTE Packet[256]);

private:
	struct IDirectInput8W *m_pDInput8; // LPDIRECTINPUT8
	struct IDirectInputDevice8W *m_pDInputDevice; // LPDIRECTINPUTDEVICE8 
};

#pragma managed
