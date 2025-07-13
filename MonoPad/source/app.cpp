
#include "app.h"
#include "mainframe.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(app);

bool app::OnInit() {
    mainFrame* mainframe = new mainFrame("MonoPad");
    mainframe->SetClientSize(wxSize(800, 500));
    mainframe->SetMinSize(wxSize(500, 300));
    mainframe->Center();
    mainframe->Show();
    return true;
}
