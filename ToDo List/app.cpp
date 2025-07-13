
#include "app.h"
#include "mainframe.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    mainFrame* frame = new mainFrame("To-Do List");
    frame->SetClientSize(400, 330);
    frame->SetIcons(wxIconBundle(wxIcon("todo.ico", wxBITMAP_TYPE_ICO)));
    frame->Center();
    frame->Show();
    return true;
}