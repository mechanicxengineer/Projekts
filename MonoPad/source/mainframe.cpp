

#include "mainframe.h"
#include "vector"
#include <fstream>
#include <sstream>
#include <wx/app.h>


mainFrame::mainFrame(const wxString& title) 
    : wxFrame(NULL, wxID_ANY, title)
{
    //-- wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX) this keywords for not resize and not maximize
    
    CreateMenuBar();
    BindEvents();
    wxTheApp->CallAfter([this] {
        OpenLastFile();
    });

    textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 
        wxTE_MULTILINE | wxTE_RICH2 | wxTE_NOHIDESEL | wxBORDER_NONE);
    if (currentFilePath.IsEmpty()) {
        textCtrl->ChangeValue("\n\n\n\n\t\t\t\t\t\t --- Welcome to monoPad --- \n\t\t\t\t\tStart typing or use File -> Open to load a file.");
    }


    statusTimer = new wxTimer(this);

    font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    font.SetPointSize(12);
    textCtrl->SetFont(font);
    Layout();
    CreateStatusBar();
    SetStatusText("Welcome to MonoPad!");
    statusTimer->Start(3000, wxTIMER_ONE_SHOT);
}

void mainFrame::CreateMenuBar() {
    auto* fileMenu = new wxMenu;
    fileMenu->Append(wxID_NEW, "&New\tCtrl+N");
    fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O");
    fileMenu->Append(wxID_SAVE, "&Save\tCtrl+S");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl+Q");

    auto* infoMenu = new wxMenu;
    infoMenu->Append(wxID_ABOUT, "&About\tF1");

    auto* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(infoMenu, "&Info");

    SetMenuBar(menuBar);

}

void mainFrame::BindEvents() {
    Bind(wxEVT_MENU, &mainFrame::OnNewFileClicked, this, wxID_NEW);
    Bind(wxEVT_MENU, &mainFrame::OnSaveClicked, this, wxID_SAVE);
    Bind(wxEVT_MENU, &mainFrame::OnOpenClicked, this, wxID_OPEN);
    Bind(wxEVT_MENU, [](wxCommandEvent&) { wxTheApp->Exit(); }, wxID_EXIT);
    Bind(wxEVT_MENU, &mainFrame::OnAboutClicked, this, wxID_ABOUT);
    Bind(wxEVT_TIMER, &mainFrame::OnStatusTimeout, this);
    
}

void mainFrame::OpenLastFile() {
    wxConfig config("MonoPad");
    wxString lastPath;
    if (config.Read("LastFilePath", &lastPath) && wxFileExists(lastPath)) {
        std::ifstream file(lastPath.ToStdString());
        std::stringstream buffer;
        if (file.is_open()) {
            buffer << file.rdbuf();
            textCtrl->ChangeValue(buffer.str());
            currentFilePath = lastPath;
            SetTitle("MonoPad - " + wxFileName(currentFilePath).GetFullName());
            SetStatusText("Reopened: " + currentFilePath);
            statusTimer->Start(3000, wxTIMER_ONE_SHOT);
        }
    }

}

void mainFrame::OnNewFileClicked(wxCommandEvent& event) {
    textCtrl->Clear();
    textCtrl->SetFocus();
    textCtrl->SetInsertionPoint(0);
}

void mainFrame::OnSaveClicked(wxCommandEvent& event) {
    if (!currentFilePath.IsEmpty()) {
        std::ofstream file(currentFilePath.ToStdString());
        if (file.is_open()) {
            file << textCtrl->GetValue().ToStdString();
            file.close();
            SetStatusText("Saved: " + currentFilePath);
            statusTimer->Start(3000, wxTIMER_ONE_SHOT);
        } else {
            SetStatusText("Failed to save file.");
        }
    } else {
        wxFileDialog saveFileDialog(this, "Save Text file", "", "",
            "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

        currentFilePath = saveFileDialog.GetPath();
        std::ofstream file(currentFilePath.ToStdString());
        if (file.is_open()) {
            file << textCtrl->GetValue().ToStdString();
            file.close();
            SetTitle("MonoPad - " + wxFileName(currentFilePath).GetFullName());
            SetStatusText("Saved: " + currentFilePath);
            statusTimer->Start(3000, wxTIMER_ONE_SHOT);

            wxConfig config("MonoPad");
            config.Write("LastFilePath", currentFilePath);
            config.Flush();
        } else {
            SetStatusText("Failed to save file.");
        }
    }
}


void mainFrame::OnOpenClicked(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open Text file", "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    std::ifstream file(openFileDialog.GetPath().ToStdString());
    std::stringstream buffer;
    if (file.is_open()) {
        buffer << file.rdbuf();
        textCtrl->SetFocus();
        textCtrl->ChangeValue(buffer.str());
        textCtrl->SetInsertionPointEnd();

        currentFilePath = openFileDialog.GetPath();
        SetTitle("MonoPad - " + wxFileName(currentFilePath).GetFullName());
        SetStatusText("Opened: " + currentFilePath);
        statusTimer->Start(3000, wxTIMER_ONE_SHOT);

        wxConfig config("MonoPad");
        config.Write("LastFilePath", currentFilePath);
        config.Flush();
    }
}

void mainFrame::OnAboutClicked(wxCommandEvent& event) {
    wxMessageBox("MonoPad\nVersion 1.0\nCopyright 2025 Manish",
                 "About MonoPad", wxOK | wxICON_INFORMATION, this);
    statusTimer->Start(3000, wxTIMER_ONE_SHOT);
}

void mainFrame::OnStatusTimeout(wxTimerEvent& event) {
    SetStatusText("");
}