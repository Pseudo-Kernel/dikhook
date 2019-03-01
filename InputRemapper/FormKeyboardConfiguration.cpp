
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

#include <Windows.h>
#include "FormKeyboardConfiguration.h"

using namespace InputRemapper;


System::Void FormKeyboardConfiguration::AddKeyMappingList(ListView ^lvList, BYTE ScanCode, BYTE MapTo)
{
	for each (ListViewItem ^item in lvList->Items)
	{
		if ((BYTE)item->Tag == ScanCode)
		{
			item->SubItems[0]->Text = gcnew String(m_Keyboard->GetKeyString(ScanCode));
			item->SubItems[1]->Text = ScanCode.ToString();
			item->SubItems[2]->Text = gcnew String(m_Keyboard->GetKeyString(MapTo));
			item->SubItems[3]->Text = MapTo.ToString();
			return;
		}
	}

	ListViewItem ^item = gcnew ListViewItem();
	item->Text = gcnew String(m_Keyboard->GetKeyString(ScanCode));
	item->SubItems->Add(ScanCode.ToString());
	item->SubItems->Add(gcnew String(m_Keyboard->GetKeyString(MapTo)));
	item->SubItems->Add(MapTo.ToString());
	item->Tag = ScanCode;
	lvList->Items->Add(item);
}

System::Void FormKeyboardConfiguration::DeleteKeyMappingList(ListView ^lvList, BYTE ScanCode)
{
	for each (ListViewItem ^item in lvList->Items)
	{
		if ((BYTE)item->Tag == ScanCode)
			item->Remove();
	}
}

System::Void FormKeyboardConfiguration::RefreshKeyMappingList(ListView ^lvList)
{
	BYTE Mapping[256];
	ZeroMemory(Mapping, sizeof(Mapping));
	m_Hook->GetKeyMapping(Mapping);

	lvList->Items->Clear();
	for (int ScanCode = 0; ScanCode < 256; ScanCode++)
	{
		if (Mapping[ScanCode] != ScanCode)
			AddKeyMappingList(lvList, ScanCode, Mapping[ScanCode]);
	}
}



System::Void FormKeyboardConfiguration::btnAddMapping_Click(System::Object^  sender, System::EventArgs^  e)
{
	m_bAddMapping = true;
	checkBoxNullMapping->Enabled = false;
	btnAddMapping->Enabled = false;
	lblKeyInputMsg->Text = "Press the 1st and 2nd key";

	m_FirstScanCode = 0;
	m_SecondScanCode = 0;
	m_WaitCount = 0;

	timerKeyDetect->Stop();
	timerKeyDetect->Interval = 10;
	timerKeyDetect->Start();
}

System::Void FormKeyboardConfiguration::timerKeyDetect_Tick(System::Object^  sender, System::EventArgs^  e)
{
	BYTE Packet[256];
	int Count = 0;
	String ^Text = String::Empty;
	String ^KeyListText = String::Empty;
	bool bFirstKey = false;
	bool bEnd = false;
	char *KeyString;
	int ScanCode = 0;

	if (!m_Keyboard->GetKeyState(Packet))
		return;

	m_WaitCount++;

	if (m_bAddMapping && m_WaitCount > 300)
	{
		lblKeyInputMsg->Text = "Canceled by timeout";
		bEnd = true;
	}

	for (int i = 0; i < 0x100; i++)
	{
		if (!(Packet[i] & 0x80))
			continue;

		ScanCode = i;

		KeyString = m_Keyboard->GetKeyString(ScanCode);
		KeyListText += gcnew String(KeyString);
		KeyListText += " ";

		Count++;
	}

	lblKeyDetectionList->Text = KeyListText;

	if (m_bAddMapping && Count == 1)
	{
		if (m_FirstScanCode == 0)
		{
			m_WaitCount = 0;
			bFirstKey = true;
			m_FirstScanCode = ScanCode;

			if (checkBoxNullMapping->Checked)
			{
				lblKeyInputMsg->Text = String::Format(
					gcnew String("Added null mapping for key: {0}"), gcnew String(KeyString));
				bFirstKey = false;
			}
			else
			{
				lblKeyInputMsg->Text = String::Format(
					gcnew String("Press the 2nd key (1st key: {0})"), gcnew String(KeyString));
			}
		}
		else
		{
			if (m_FirstScanCode == ScanCode)
			{
				Count = 0;
			}
			else
			{
				bFirstKey = false;
				m_SecondScanCode = ScanCode;
				lblKeyInputMsg->Text = String::Format(
					gcnew String("1st key: {0}, 2nd key: {1}"),
					gcnew String(m_Keyboard->GetKeyString(m_FirstScanCode)),
					gcnew String(KeyString));

			}
		}
	}


	if (m_bAddMapping)
	{
		if (Count == 0)
		{
			// Do nothing
		}
		else if (Count == 1)
		{
			if (bFirstKey)
			{
				// Do nothing
			}
			else
			{
				// Add mapping.
				AddKeyMappingList(lvKeyRemappingList, m_FirstScanCode, m_SecondScanCode);
				bEnd = true;
			}
		}
		else
		{
			lblKeyInputMsg->Text = "Canceled by invalid input";
			bEnd = true;
		}
	}

	if (bEnd)
	{
		btnAddMapping->Enabled = true;
		checkBoxNullMapping->Enabled = true;
		m_bAddMapping = false;
	}
}

System::Void FormKeyboardConfiguration::FormKeyboardConfiguration_Load(System::Object^  sender, System::EventArgs^  e)
{
	if (!m_Keyboard->CreateInstance((HMODULE)(void *)IntPtr::Zero,
		(HWND)(void *)this->Handle, TRUE, TRUE))
	{
		lblKeyDetectionList->Text = "Failed to initialize DInput8";
		((Form ^)sender)->Close();
		return;
	}

	RefreshKeyMappingList(lvKeyRemappingList);

	timerKeyDetect->Start();
}

System::Void FormKeyboardConfiguration::FormKeyboardConfiguration_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
{
	delete m_Keyboard;
	m_Keyboard = nullptr;
}

System::Void FormKeyboardConfiguration::FormKeyboardConfiguration_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	timerKeyDetect->Stop();
}

System::Void FormKeyboardConfiguration::btnDeleteMapping_Click(System::Object^  sender, System::EventArgs^  e)
{
	for each (ListViewItem ^item in lvKeyRemappingList->Items)
	{
		if (!item->Selected)
			continue;

		item->Remove();
	}
}

System::Void FormKeyboardConfiguration::btnDeleteMappingAll_Click(System::Object^  sender, System::EventArgs^  e)
{
	lvKeyRemappingList->Items->Clear();
}

System::Void FormKeyboardConfiguration::btnApplySettings_Click(System::Object^  sender, System::EventArgs^  e)
{
	m_Hook->DeleteKeyMapping();

	for each (ListViewItem ^item in lvKeyRemappingList->Items)
	{
		BYTE ScanCode = (BYTE)item->Tag;
		BYTE MapTo = BYTE::Parse(item->SubItems[3]->Text);
		m_Hook->SetKeyMapping(ScanCode, MapTo);
	}
}
