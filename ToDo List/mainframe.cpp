
#include "mainframe.h"
#include "Task.h"
#include <vector>
#include <string>


mainFrame::mainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    CreateControls();
    SetupSizers();
    BindEventHandlers();
    AddSaveTasks();
}

void mainFrame::CreateControls() {
    wxFont headlineFont(wxFontInfo(wxSize(0, 24)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 14)));

    panel = new wxPanel(this);
    panel->SetFont(mainFont);

    headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List");
    headlineText->SetFont(headlineFont);

    inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    addButton = new wxButton(panel, wxID_ANY, "Add");
    checkListBox = new wxCheckListBox(panel, wxID_ANY);
    clearButton = new wxButton(panel, wxID_ANY, "Clear");

}

void mainFrame::SetupSizers() {

    //     main
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(headlineText, wxSizerFlags().CenterHorizontal());
    mainSizer->AddSpacer(20);

    //  input
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer->Add(inputField, wxSizerFlags().Proportion(1));
    inputSizer->AddSpacer(5);
    inputSizer->Add(addButton);

    mainSizer->Add(inputSizer, wxSizerFlags().Expand());
    mainSizer->AddSpacer(5);
    mainSizer->Add(checkListBox, wxSizerFlags().Expand().Proportion(1));
    mainSizer->AddSpacer(5);
    mainSizer->Add(clearButton);

    wxGridSizer* outerSizer = new wxGridSizer(1);
    outerSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, 25).Expand());

    panel->SetSizer(outerSizer);
    outerSizer->SetSizeHints(this);
}

void mainFrame::BindEventHandlers() {
    addButton->Bind(wxEVT_BUTTON, &mainFrame::OnAddButtonClicked, this);
    inputField->Bind(wxEVT_TEXT_ENTER, &mainFrame::OnInputEnter, this);
    checkListBox->Bind(wxEVT_KEY_DOWN, &mainFrame::OnListKeyDown, this);
    clearButton->Bind(wxEVT_BUTTON, &mainFrame::OnClearButtonClicked, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &mainFrame::OnWindowClosed, this);
}

void mainFrame::AddTaskFromInput() {
    wxString description = inputField->GetValue();
    if (!description.IsEmpty()) {
        checkListBox->Insert(description, checkListBox->GetCount());
        inputField->Clear();
    }
    inputField->SetFocus();
}

void mainFrame::DeleteSelectedTasks() {
    int selectedIndex = checkListBox->GetSelection();
    if (selectedIndex == wxNOT_FOUND) return;
    checkListBox->Delete(selectedIndex);
}

void mainFrame::MoveSelectedTask(int offset) {
    int selectedIndex = checkListBox->GetSelection();
    if (selectedIndex == wxNOT_FOUND) return;
    
    int newIndex = selectedIndex + offset;
    if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {
        SwapTasks(selectedIndex, newIndex);
        checkListBox->SetSelection(newIndex, true);
    }
}

void mainFrame::SwapTasks(int index1, int index2) {
    Task task1{ checkListBox->GetString(index1).ToStdString(), checkListBox->IsChecked(index1) };
    Task task2{ checkListBox->GetString(index2).ToStdString(), checkListBox->IsChecked(index2) };

    checkListBox->SetString(index1, task2.description);
    checkListBox->Check(index1, task2.done);

    checkListBox->SetString(index2, task1.description);
    checkListBox->Check(index2, task1.done);
}

void mainFrame::AddSaveTasks() {
    std::vector<Task> tasks = LoadTasksFromFile(GetDocumentsPath());

    for (const Task& task : tasks) {
        int index = checkListBox->GetCount();
        checkListBox->Insert(task.description, index);
        checkListBox->Check(index, task.done);
    }
}

void mainFrame::OnAddButtonClicked(wxCommandEvent& event) {
    AddTaskFromInput();
}

void mainFrame::OnInputEnter(wxCommandEvent& event) {
    AddTaskFromInput();
}

void mainFrame::OnListKeyDown(wxKeyEvent& event) {
    switch (event.GetKeyCode())
    {
    case WXK_DELETE:
        DeleteSelectedTasks();
        break;
    case WXK_UP:
        MoveSelectedTask(-1);
        break;
    case WXK_DOWN:
        MoveSelectedTask(1);
        break;
    default:
        break;
    }
}

void mainFrame::OnClearButtonClicked(wxCommandEvent& event) {
    if (checkListBox->IsEmpty()) return;

    wxMessageDialog dialog(this, "Are you sure you want to clear the list?", 
        "Clear List", wxYES_NO | wxNO_DEFAULT);
    int result = dialog.ShowModal();
    if (result == wxID_YES) {
        checkListBox->Clear();
    }
}

void mainFrame::OnWindowClosed(wxCloseEvent& event) {
    std::vector<Task> tasks;

    for (int i = 0; i < checkListBox->GetCount(); ++i) {
        Task task;
        task.description = checkListBox->GetString(i);
        task.done = checkListBox->IsChecked(i);
        tasks.push_back(task);
    }

    SaveTasksToFile(tasks, GetDocumentsPath());
    event.Skip();
}