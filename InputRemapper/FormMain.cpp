
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
#include <windows.h>
#include <dinput.h>
#include <TlHelp32.h>

#include "DInputKeyboard.h"
#include "DInputKeyboardHook.h"
#include "FormKeyboardConfiguration.h"
#include "FormMain.h"

using namespace InputRemapper;
using namespace System::Windows::Forms;

System::Void FormMain::RefreshProcessList(System::Object^ lvProcessList)
{
	HANDLE SnapshotHandle;
	PROCESSENTRY32 Entry;
	BOOL Found;

	ListView ^List = (ListView ^)lvProcessList;

	SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!SnapshotHandle)
		return;

	Entry.dwSize = sizeof(Entry);
	Found = Process32First(SnapshotHandle, &Entry);

	List->Items->Clear();

	while (Found)
	{
		String ^ProcessName = gcnew String((wchar_t *)Entry.szExeFile);
		Int32 ProcessId = Entry.th32ProcessID;

		ListViewItem ^Item = gcnew ListViewItem(ProcessName);
		Item->SubItems->Add(ProcessId.ToString());

		if (m_Hook->IsHookInstalled(ProcessId))
			Item->SubItems->Add("Installed");

		List->Items->Add(Item);

		Found = Process32Next(SnapshotHandle, &Entry);
	}

	CloseHandle(SnapshotHandle);
}

System::Void FormMain::btnInstallHook_Click(System::Object^  sender, System::EventArgs^  e)
{
	for each (ListViewItem ^item in lvProcessList->Items)
	{
		if (item->Selected)
		{
			Int32 ProcessId = Int32::Parse(item->SubItems[1]->Text);

			if (ProcessId == GetCurrentProcessId())
				break; // silent fail

			if (m_Hook->IsHookInstalled(ProcessId))
			{
				MessageBox::Show("Keyboard remapping is already enabled for this process.", "Fail",
					MessageBoxButtons::OK,
					System::Windows::Forms::MessageBoxIcon::Error);
				break;
			}

			String ^Text = String::Format(
				"This will enable keyboard remapping to:\n\n"
				"{0} (PID {1})\n\n"
				"Do you want to continue?", 
				item->Text, item->SubItems[1]->Text);

			if (MessageBox::Show(Text, item->Text, MessageBoxButtons::YesNo, 
				System::Windows::Forms::MessageBoxIcon::Question)
				== System::Windows::Forms::DialogResult::Yes)
			{
				if (!m_Hook->InstallHook(ProcessId))
				{
					MessageBox::Show("Failed to enable remapping", "Fail",
						MessageBoxButtons::OK, 
						System::Windows::Forms::MessageBoxIcon::Error);
				}
			}

			break;
		}
	}
}

System::Void FormMain::btnUninstallHook_Click(System::Object^  sender, System::EventArgs^  e)
{
	for each (ListViewItem ^item in lvProcessList->Items)
	{
		if (item->Selected)
		{
			Int32 ProcessId = Int32::Parse(item->SubItems[1]->Text);

			if (ProcessId == GetCurrentProcessId())
				break; // silent fail

			String ^Text = String::Format(
				"This will disable keyboard remapping to:\n\n"
				"{0} (PID {1})\n\n"
				"Do you want to continue?",
				item->Text, item->SubItems[1]->Text);

			if (MessageBox::Show(Text, item->Text, MessageBoxButtons::YesNo,
				System::Windows::Forms::MessageBoxIcon::Question)
				== System::Windows::Forms::DialogResult::Yes)
			{
				if (!m_Hook->UninstallHook(ProcessId))
				{
					MessageBox::Show("Failed to disable remapping", "Fail",
						MessageBoxButtons::OK,
						System::Windows::Forms::MessageBoxIcon::Error);
				}
			}

			break;
		}
	}
}

System::Void FormMain::btnConfig_Click(System::Object^  sender, System::EventArgs^  e)
{
	FormKeyboardConfiguration ^Form = gcnew FormKeyboardConfiguration(m_Hook);
	Form->ShowDialog();
}

System::Void FormMain::btnRefreshList_Click(System::Object^  sender, System::EventArgs^  e)
{
	RefreshProcessList(lvProcessList);
}

System::Void FormMain::FormMain_Load(System::Object^  sender, System::EventArgs^  e)
{
	if (!m_Hook->Initialize())
	{
		MessageBox::Show("Failed to initialize", "Failed",
			System::Windows::Forms::MessageBoxButtons::OK,
			System::Windows::Forms::MessageBoxIcon::Error);
		Close();

		Application::Exit();
		return;
	}

	RefreshProcessList(lvProcessList);
}

System::Void FormMain::FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
{
	if (m_Hook)
	{
		delete m_Hook;
		m_Hook = nullptr;
	}
}

System::Void FormMain::btnLoadConfig_Click(System::Object^  sender, System::EventArgs^  e)
{
	FILE *ConfigFile = nullptr;
	BYTE Mapping[256];

	if (MessageBox::Show(
		"Unsaved changes will be lost.\n\n"
		"Do you want to continue?", 
		"Load Configuration",
		MessageBoxButtons::YesNo, MessageBoxIcon::Exclamation) !=
		::DialogResult::Yes)
		return;

	if (fopen_s(&ConfigFile, "keyconfig.cfg", "rb") || !ConfigFile)
	{
		MessageBox::Show("Failed to open configuration", "Failed", 
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	fread(Mapping, sizeof(Mapping), 1, ConfigFile);
	fclose(ConfigFile);

	if (!m_Hook->SetKeyMapping(Mapping))
	{
		MessageBox::Show("Failed to apply configuration", "Failed",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	MessageBox::Show("Configuration loaded successfully.", "Succeeded",
		MessageBoxButtons::OK, MessageBoxIcon::Information);
}

System::Void FormMain::btnSaveConfig_Click(System::Object^  sender, System::EventArgs^  e)
{
	FILE *ConfigFile = nullptr;
	BYTE Mapping[256];

	if (MessageBox::Show(
		"Do you want to save configuration to file?",
		"Save Configuration",
		MessageBoxButtons::YesNo, MessageBoxIcon::Information) !=
		::DialogResult::Yes)
		return;

	if (fopen_s(&ConfigFile, "keyconfig.cfg", "wb") || !ConfigFile)
	{
		MessageBox::Show("Failed to create configuration", "Failed",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	if (!m_Hook->GetKeyMapping(Mapping))
	{
		MessageBox::Show("Failed to get current configuration", "Failed",
			MessageBoxButtons::OK, MessageBoxIcon::Error);

		fclose(ConfigFile);
		return;
	}

	fwrite(Mapping, sizeof(Mapping), 1, ConfigFile);
	fclose(ConfigFile);

	MessageBox::Show("Configuration saved successfully.", "Succeeded",
		MessageBoxButtons::OK, MessageBoxIcon::Information);
}
