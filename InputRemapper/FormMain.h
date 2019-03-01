
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

namespace InputRemapper {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormMain
	/// </summary>
	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		FormMain(void)
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
		~FormMain()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lblTitle;
	private: System::Windows::Forms::ListView^  lvProcessList;
	protected:

	private: System::Windows::Forms::ColumnHeader^  colProcessName;
	private: System::Windows::Forms::ColumnHeader^  colProcessId;
	private: System::Windows::Forms::ColumnHeader^  colRemappedState;
	private: System::Windows::Forms::SplitContainer^  splitMainUI;
	private: System::Windows::Forms::Button^  btnUninstallHook;
	private: System::Windows::Forms::Button^  btnInstallHook;
	private: System::Windows::Forms::Button^  btnConfig;
	private: System::Windows::Forms::Button^  btnRefreshList;
	private: System::Windows::Forms::Button^  btnSaveConfig;
	private: System::Windows::Forms::Button^  btnLoadConfig;
	private: System::Windows::Forms::ToolTip^  toolTipHelp;
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
			this->lvProcessList = (gcnew System::Windows::Forms::ListView());
			this->colProcessName = (gcnew System::Windows::Forms::ColumnHeader());
			this->colProcessId = (gcnew System::Windows::Forms::ColumnHeader());
			this->colRemappedState = (gcnew System::Windows::Forms::ColumnHeader());
			this->splitMainUI = (gcnew System::Windows::Forms::SplitContainer());
			this->btnSaveConfig = (gcnew System::Windows::Forms::Button());
			this->btnLoadConfig = (gcnew System::Windows::Forms::Button());
			this->btnRefreshList = (gcnew System::Windows::Forms::Button());
			this->btnConfig = (gcnew System::Windows::Forms::Button());
			this->btnUninstallHook = (gcnew System::Windows::Forms::Button());
			this->btnInstallHook = (gcnew System::Windows::Forms::Button());
			this->toolTipHelp = (gcnew System::Windows::Forms::ToolTip(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitMainUI))->BeginInit();
			this->splitMainUI->Panel1->SuspendLayout();
			this->splitMainUI->Panel2->SuspendLayout();
			this->splitMainUI->SuspendLayout();
			this->SuspendLayout();
			// 
			// lblTitle
			// 
			this->lblTitle->Location = System::Drawing::Point(15, 9);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(736, 33);
			this->lblTitle->TabIndex = 0;
			this->lblTitle->Text = L"This program intercepts DirectInput8 and remaps the keyboard inputs.";
			// 
			// lvProcessList
			// 
			this->lvProcessList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {
				this->colProcessName,
					this->colProcessId, this->colRemappedState
			});
			this->lvProcessList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lvProcessList->FullRowSelect = true;
			this->lvProcessList->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->lvProcessList->Location = System::Drawing::Point(0, 0);
			this->lvProcessList->MultiSelect = false;
			this->lvProcessList->Name = L"lvProcessList";
			this->lvProcessList->Size = System::Drawing::Size(547, 463);
			this->lvProcessList->TabIndex = 1;
			this->toolTipHelp->SetToolTip(this->lvProcessList, L"You can select the target process here.");
			this->lvProcessList->UseCompatibleStateImageBehavior = false;
			this->lvProcessList->View = System::Windows::Forms::View::Details;
			// 
			// colProcessName
			// 
			this->colProcessName->Text = L"Process";
			this->colProcessName->Width = 224;
			// 
			// colProcessId
			// 
			this->colProcessId->Text = L"PID";
			this->colProcessId->Width = 92;
			// 
			// colRemappedState
			// 
			this->colRemappedState->Text = L"Installed\?";
			this->colRemappedState->Width = 100;
			// 
			// splitMainUI
			// 
			this->splitMainUI->IsSplitterFixed = true;
			this->splitMainUI->Location = System::Drawing::Point(15, 45);
			this->splitMainUI->Name = L"splitMainUI";
			// 
			// splitMainUI.Panel1
			// 
			this->splitMainUI->Panel1->Controls->Add(this->lvProcessList);
			// 
			// splitMainUI.Panel2
			// 
			this->splitMainUI->Panel2->Controls->Add(this->btnSaveConfig);
			this->splitMainUI->Panel2->Controls->Add(this->btnLoadConfig);
			this->splitMainUI->Panel2->Controls->Add(this->btnRefreshList);
			this->splitMainUI->Panel2->Controls->Add(this->btnConfig);
			this->splitMainUI->Panel2->Controls->Add(this->btnUninstallHook);
			this->splitMainUI->Panel2->Controls->Add(this->btnInstallHook);
			this->splitMainUI->Size = System::Drawing::Size(736, 463);
			this->splitMainUI->SplitterDistance = 547;
			this->splitMainUI->TabIndex = 2;
			// 
			// btnSaveConfig
			// 
			this->btnSaveConfig->Location = System::Drawing::Point(1, 233);
			this->btnSaveConfig->Name = L"btnSaveConfig";
			this->btnSaveConfig->Size = System::Drawing::Size(181, 40);
			this->btnSaveConfig->TabIndex = 0;
			this->btnSaveConfig->Text = L"Save Configuration";
			this->toolTipHelp->SetToolTip(this->btnSaveConfig, L"Saves the configuration to the file.");
			this->btnSaveConfig->UseVisualStyleBackColor = true;
			this->btnSaveConfig->Click += gcnew System::EventHandler(this, &FormMain::btnSaveConfig_Click);
			// 
			// btnLoadConfig
			// 
			this->btnLoadConfig->Location = System::Drawing::Point(1, 187);
			this->btnLoadConfig->Name = L"btnLoadConfig";
			this->btnLoadConfig->Size = System::Drawing::Size(181, 40);
			this->btnLoadConfig->TabIndex = 0;
			this->btnLoadConfig->Text = L"Load Configuration";
			this->toolTipHelp->SetToolTip(this->btnLoadConfig, L"Loads the configuration from the file.");
			this->btnLoadConfig->UseVisualStyleBackColor = true;
			this->btnLoadConfig->Click += gcnew System::EventHandler(this, &FormMain::btnLoadConfig_Click);
			// 
			// btnRefreshList
			// 
			this->btnRefreshList->Location = System::Drawing::Point(1, 95);
			this->btnRefreshList->Name = L"btnRefreshList";
			this->btnRefreshList->Size = System::Drawing::Size(181, 40);
			this->btnRefreshList->TabIndex = 0;
			this->btnRefreshList->Text = L"Refresh List";
			this->toolTipHelp->SetToolTip(this->btnRefreshList, L"Refresh the process list.");
			this->btnRefreshList->UseVisualStyleBackColor = true;
			this->btnRefreshList->Click += gcnew System::EventHandler(this, &FormMain::btnRefreshList_Click);
			// 
			// btnConfig
			// 
			this->btnConfig->Location = System::Drawing::Point(1, 141);
			this->btnConfig->Name = L"btnConfig";
			this->btnConfig->Size = System::Drawing::Size(181, 40);
			this->btnConfig->TabIndex = 0;
			this->btnConfig->Text = L"Key Configuration";
			this->toolTipHelp->SetToolTip(this->btnConfig, L"Opens the key mapping configuration dialog.");
			this->btnConfig->UseVisualStyleBackColor = true;
			this->btnConfig->Click += gcnew System::EventHandler(this, &FormMain::btnConfig_Click);
			// 
			// btnUninstallHook
			// 
			this->btnUninstallHook->Location = System::Drawing::Point(1, 49);
			this->btnUninstallHook->Name = L"btnUninstallHook";
			this->btnUninstallHook->Size = System::Drawing::Size(181, 40);
			this->btnUninstallHook->TabIndex = 0;
			this->btnUninstallHook->Text = L"Uninstall Hook";
			this->toolTipHelp->SetToolTip(this->btnUninstallHook, L"Uninstall Hook for selected process.");
			this->btnUninstallHook->UseVisualStyleBackColor = true;
			this->btnUninstallHook->Click += gcnew System::EventHandler(this, &FormMain::btnUninstallHook_Click);
			// 
			// btnInstallHook
			// 
			this->btnInstallHook->Location = System::Drawing::Point(1, 3);
			this->btnInstallHook->Name = L"btnInstallHook";
			this->btnInstallHook->Size = System::Drawing::Size(181, 40);
			this->btnInstallHook->TabIndex = 0;
			this->btnInstallHook->Text = L"Install Hook";
			this->toolTipHelp->SetToolTip(this->btnInstallHook, L"Install Hook for selected process.");
			this->btnInstallHook->UseVisualStyleBackColor = true;
			this->btnInstallHook->Click += gcnew System::EventHandler(this, &FormMain::btnInstallHook_Click);
			// 
			// toolTipHelp
			// 
			this->toolTipHelp->IsBalloon = true;
			this->toolTipHelp->ToolTipIcon = System::Windows::Forms::ToolTipIcon::Info;
			this->toolTipHelp->ToolTipTitle = L"Help";
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(763, 520);
			this->Controls->Add(this->splitMainUI);
			this->Controls->Add(this->lblTitle);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"FormMain";
			this->Text = L"DirectInput8 Keyboard Input Remapper";
			this->TopMost = true;
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormMain::FormMain_FormClosed);
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->splitMainUI->Panel1->ResumeLayout(false);
			this->splitMainUI->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitMainUI))->EndInit();
			this->splitMainUI->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
		private: CDInputKeyboardHook *m_Hook = new CDInputKeyboardHook();

		private: System::Void FormMain::RefreshProcessList(System::Object^ lvProcessList);

		private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnConfig_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
		private: System::Void btnRefreshList_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnInstallHook_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnUninstallHook_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnLoadConfig_Click(System::Object^  sender, System::EventArgs^  e);
		private: System::Void btnSaveConfig_Click(System::Object^  sender, System::EventArgs^  e);
	};
}
