#pragma once

#include <wx/wx.h>

class mainFrame : public wxFrame {

public:
    mainFrame(const wxString& title);

private:
    //  Properties
    wxPanel* panel;
    wxStaticText* headlineText;
    wxTextCtrl* inputField;
    wxButton* addButton;
    wxCheckListBox* checkListBox;
    wxButton* clearButton;
    
    //  Functions
    void CreateControls();
    void SetupSizers();
    void BindEventHandlers();
    void AddTaskFromInput();
    void DeleteSelectedTasks();
    void MoveSelectedTask(int offset);
    void SwapTasks(int index1, int index2);
    void AddSaveTasks();
    
    //  Event Handlers
    void OnAddButtonClicked(wxCommandEvent& event);
    void OnInputEnter(wxCommandEvent& event);
    void OnListKeyDown(wxKeyEvent& event);
    void OnClearButtonClicked(wxCommandEvent& event);
    void OnWindowClosed(wxCloseEvent& event);
};