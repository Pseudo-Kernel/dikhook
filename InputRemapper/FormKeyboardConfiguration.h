
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

#include "DInputKeyboard.h"
#include "DInputKeyboardHook.h"

namespace InputRemapper {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormKeyboardConfiguration
	/// </summary>
	public ref class FormKeyboardConfiguration : public System::Windows::Forms::Form
	{
	public:
		FormKeyboardConfiguration(CDInputKeyboardHook *Hook) : m_Hook(Hook)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormKeyboardConfiguration()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lblTitle;
	private: System::Windows::Forms::ListView^  lvKeyRemappingList;
	protected:

	private: System::Windows::Forms::ColumnHeader^  colOriginalKey;
	private: System::Windows::Forms::ColumnHeader^  colRemappedKey;
	private: System::Windows::Forms::Button^  btnAddMapping;
	private: System::Windows::Forms::Button^  btnDeleteMapping;
	private: System::Windows::Forms::Button^  btnDeleteMappingAll;
	private: System::Windows::Forms::Timer^  timerKeyDetect;
	private: System::Windows::Forms::Label^  lblKeyInputMsg;
	private: System::Windows::Forms::Label^  lblKeyDetectionList;
	private: System::Windows::Forms::Button^  btnApplySettings;
	private: System::Windows::Forms::ColumnHeader^  colOriginalScanCode;
	private: System::Windows::Forms::ColumnHeader^  colRemappedScanCode;
	private: System::Windows::Forms::ToolTip^  toolTipHelp;
	private: System::Windows::Forms::CheckBox^  checkBoxNullMapping;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lvKeyRemappingList = (gcnew System::Windows::Forms::ListView());
			this->colOriginalKey = (gcnew System::Windows::Forms::ColumnHeader());
			this->colOriginalScanCode = (gcnew System::Windows::Forms::ColumnHeader());
			this->colRemappedKey = (gcnew System::Windows::Forms::ColumnHeader());
			this->colRemappedScanCode = (gcnew System::Windows::Forms::ColumnHeader());
			this->btnAddMapping = (gcnew System::Windows::Forms::Button());
			this->btnDeleteMapping = (gcnew System::Windows::Forms::Button());
			this->btnDeleteMappingAll = (gcnew System::Windows::Forms::Button());
			this->timerKeyDetect = (gcnew System::Windows::Forms::Timer(this->components));
			this->lblKeyInputMsg = (gcnew System::Windows::Forms::Label());
			this->lblKeyDetectionList = (gcnew System::Windows::Forms::Label());
			this->btnApplySettings = (gcnew System::Windows::Forms::Button());
			this->toolTipHelp = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->checkBoxNullMapping = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// lblTitle
			// 
			this->lblTitle->Location = System::Drawing::Point(9, 9);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(614, 25);
			this->lblTitle->TabIndex = 0;
			this->lblTitle->Text = L"Yon can add or delete the key mapping here.";
			// 
			// lvKeyRemappingList
			// 
			this->lvKeyRemappingList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {
				this->colOriginalKey,
					this->colOriginalScanCode, this->colRemappedKey, this->colRemappedScanCode
			});
			this->lvKeyRemappingList->FullRowSelect = true;
			this->lvKeyRemappingList->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->lvKeyRemappingList->Location = System::Drawing::Point(12, 37);
			this->lvKeyRemappingList->MultiSelect = false;
			this->lvKeyRemappingList->Name = L"lvKeyRemappingList";
			this->lvKeyRemappingList->Size = System::Drawing::Size(611, 238);
			this->lvKeyRemappingList->TabIndex = 1;
			this->toolTipHelp->SetToolTip(this->lvKeyRemappingList, L"Key mapping list.");
			this->lvKeyRemappingList->UseCompatibleStateImageBehavior = false;
			this->lvKeyRemappingList->View = System::Windows::Forms::View::Details;
			// 
			// colOriginalKey
			// 
			this->colOriginalKey->Text = L"Original Key";
			this->colOriginalKey->Width = 120;
			// 
			// colOriginalScanCode
			// 
			this->colOriginalScanCode->Text = L"Original Scancode";
			this->colOriginalScanCode->Width = 140;
			// 
			// colRemappedKey
			// 
			this->colRemappedKey->Text = L"Remapped Key";
			this->colRemappedKey->Width = 120;
			// 
			// colRemappedScanCode
			// 
			this->colRemappedScanCode->Text = L"Remapped Scancode";
			this->colRemappedScanCode->Width = 140;
			// 
			// btnAddMapping
			// 
			this->btnAddMapping->Location = System::Drawing::Point(13, 282);
			this->btnAddMapping->Name = L"btnAddMapping";
			this->btnAddMapping->Size = System::Drawing::Size(148, 39);
			this->btnAddMapping->TabIndex = 2;
			this->btnAddMapping->Text = L"Add Mapping";
			this->toolTipHelp->SetToolTip(this->btnAddMapping, L"Adds the keyboard mapping.");
			this->btnAddMapping->UseVisualStyleBackColor = true;
			this->btnAddMapping->Click += gcnew System::EventHandler(this, &FormKeyboardConfiguration::btnAddMapping_Click);
			// 
			// btnDeleteMapping
			// 
			this->btnDeleteMapping->Location = System::Drawing::Point(167, 282);
			this->btnDeleteMapping->Name = L"btnDeleteMapping";
			this->btnDeleteMapping->Size = System::Drawing::Size(148, 39);
			this->btnDeleteMapping->TabIndex = 2;
			this->btnDeleteMapping->Text = L"Delete Mapping";
			this->toolTipHelp->SetToolTip(this->btnDeleteMapping, L"Deletes the keyboard mapping.");
			this->btnDeleteMapping->UseVisualStyleBackColor = true;
			this->btnDeleteMapping->Click += gcnew System::EventHandler(this, &FormKeyboardConfiguration::btnDeleteMapping_Click);
			// 
			// btnDeleteMappingAll
			// 
			this->btnDeleteMappingAll->Location = System::Drawing::Point(321, 282);
			this->btnDeleteMappingAll->Name = L"btnDeleteMappingAll";
			this->btnDeleteMappingAll->Size = System::Drawing::Size(148, 39);
			this->btnDeleteMappingAll->TabIndex = 2;
			this->btnDeleteMappingAll->Text = L"Delete All Mapping";
			this->toolTipHelp->SetToolTip(this->btnDeleteMappingAll, L"Deletes the all keyboard mappings.");
			this->btnDeleteMappingAll->UseVisualStyleBackColor = true;
			this->btnDeleteMappingAll->Click += gcnew System::EventHandler(this, &FormKeyboardConfiguration::btnDeleteMappingAll_Click);
			// 
			// timerKeyDetect
			// 
			this->timerKeyDetect->Tick += gcnew System::EventHandler(this, &FormKeyboardConfiguration::timerKeyDetect_Tick);
			// 
			// lblKeyInputMsg
			// 
			this->lblKeyInputMsg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblKeyInputMsg->Location = System::Drawing::Point(12, 361);
			this->lblKeyInputMsg->Name = L"lblKeyInputMsg";
			this->lblKeyInputMsg->Size = System::Drawing::Size(614, 27);
			this->lblKeyInputMsg->TabIndex = 3;
			// 
			// lblKeyDetectionList
			// 
			this->lblKeyDetectionList->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblKeyDetectionList->Location = System::Drawing::Point(12, 400);
			this->lblKeyDetectionList->Name = L"lblKeyDetectionList";
			this->lblKeyDetectionList->Size = System::Drawing::Size(614, 73);
			this->lblKeyDetectionList->TabIndex = 3;
			this->toolTipHelp->SetToolTip(this->lblKeyDetectionList, L"You can test the original keyboard inputs here.");
			// 
			// btnApplySettings
			// 
			this->btnApplySettings->Location = System::Drawing::Point(475, 282);
			this->btnApplySettings->Name = L"btnApplySettings";
			this->btnApplySettings->Size = System::Drawing::Size(148, 39);
			this->btnApplySettings->TabIndex = 2;
			this->btnApplySettings->Text = L"Apply";
			this->toolTipHelp->SetToolTip(this->btnApplySettings, L"Applies mapping to process. If you want to discard the changed settings, DO NOT c"
				L"lick it.");
			this->btnApplySettings->UseVisualStyleBackColor = true;
			this->btnApplySettings->Click += gcnew System::EventHandler(this, &FormKeyboardConfiguration::btnApplySettings_Click);
			// 
			// toolTipHelp
			// 
			this->toolTipHelp->IsBalloon = true;
			this->toolTipHelp->ToolTipIcon = System::Windows::Forms::ToolTipIcon::Info;
			this->toolTipHelp->ToolTipTitle = L"Help";
			// 
			// checkBoxNullMapping
			// 
			this->checkBoxNullMapping->AutoSize = true;
			this->checkBoxNullMapping->Location = System::Drawing::Point(14, 329);
			this->checkBoxNullMapping->Name = L"checkBoxNullMapping";
			this->checkBoxNullMapping->Size = System::Drawing::Size(112, 19);
			this->checkBoxNullMapping->TabIndex = 4;
			this->checkBoxNullMapping->Text = L"Null Mapping";
			this->toolTipHelp->SetToolTip(this->checkBoxNullMapping, L"Check if you want to add null mapping.");
			this->checkBoxNullMapping->UseVisualStyleBackColor = true;
			// 
			// FormKeyboardConfiguration
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(634, 481);
			this->Controls->Add(this->checkBoxNullMapping);
			this->Controls->Add(this->lblKeyDetectionList);
			this->Controls->Add(this->lblKeyInputMsg);
			this->Controls->Add(this->btnApplySettings);
			this->Controls->Add(this->btnDeleteMappingAll);
			this->Controls->Add(this->btnDeleteMapping);
			this->Controls->Add(this->btnAddMapping);
			this->Controls->Add(this->lvKeyRemappingList);
			this->Controls->Add(this->lblTitle);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"FormKeyboardConfiguration";
			this->Text = L"Key Mapping Configuration";
			this->TopMost = true;
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormKeyboardConfiguration::FormKeyboardConfiguration_FormClosing);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormKeyboardConfiguration::FormKeyboardConfiguration_FormClosed);
			this->Load += gcnew System::EventHandler(this, &FormKeyboardConfiguration::FormKeyboardConfiguration_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		private: CDInputKeyboard *m_Keyboard = new CDInputKeyboard();
		private: CDInputKeyboardHook *m_Hook = nullptr;
		private: BYTE m_FirstScanCode = 0;
		private: BYTE m_SecondScanCode = 0;
		private: int m_WaitCount = 0;
		private: bool m_bAddMapping = false;

		private: System::Void RefreshKeyMappingList(ListView ^lvList);
		private: System::Void AddKeyMappingList(ListView ^lvList, BYTE ScanCode, BYTE MapTo);
		private: System::Void DeleteKeyMappingList(ListView ^lvList, BYTE ScanCode);

		private: System::Void btnAddMapping_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void timerKeyDetect_Tick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void FormKeyboardConfiguration_Load(System::Object^  sender, System::EventArgs^  e);
		private: System::Void FormKeyboardConfiguration_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
		private: System::Void FormKeyboardConfiguration_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
		private: System::Void btnDeleteMapping_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnDeleteMappingAll_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnApplySettings_Click(System::Object^  sender, System::EventArgs^  e);
	};
}
