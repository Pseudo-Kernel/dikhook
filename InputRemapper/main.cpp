
#include <cstdio>
#include <windows.h>

#include "DInputKeyboard.h"
#include "DInputKeyboardHook.h"
#include "FormKeyboardConfiguration.h"
#include "FormMain.h"

using namespace System;

#if 1
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

INT
WINAPI
wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCommandLine,
	IN INT nCmdShow)
{
	InputRemapper::FormMain ^Main = gcnew InputRemapper::FormMain();
	Main->ShowDialog();

	return 0;
}

#else

int main()
{
	FreeConsole();

	InputRemapper::FormMain ^Main = gcnew InputRemapper::FormMain();
	Main->ShowDialog();

	return 0;
}

#endif

