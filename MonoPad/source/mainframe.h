
#pragma once

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/config.h>

class mainFrame : public wxFrame {
public:
    mainFrame(const wxString& title);

    void CreateMenuBar();
    void BindEvents();
    void OpenLastFile();

    void OnNewFileClicked(wxCommandEvent& event);
    void OnSaveClicked(wxCommandEvent& event);
    void OnOpenClicked(wxCommandEvent& event);
    void OnAboutClicked(wxCommandEvent& event);
    void OnStatusTimeout(wxTimerEvent& event);

private:
    wxTextCtrl* textCtrl = nullptr;
    wxFont font;
    wxString currentFilePath;
    wxTimer* statusTimer = nullptr;
};
