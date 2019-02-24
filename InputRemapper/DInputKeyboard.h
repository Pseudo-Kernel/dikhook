
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
