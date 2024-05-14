#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "sqlite/sqlite3.h"
#include <string.h>

using namespace std;

struct MyFrame : public wxFrame {
    wxHtmlWindow* htmlWindow;
    wxPanel* panel;
    wxTextCtrl* searchCtrl;
    wxBoxSizer* sizer;
    wxString* str;

    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;

    static int callback(void* data, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
            *(static_cast<wxString*>(data)) += wxString(argv[i], wxConvUTF8);
        };
        return 0;
    }

    MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
        rc = sqlite3_open("data.db", &db);

        panel = new wxPanel(this, wxID_ANY);
        searchCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        searchCtrl->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnSearch, this);
        htmlWindow = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);

        str = new wxString("", wxConvUTF8);

        // Set up a sizer to manage layout
        sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(searchCtrl, 0, wxEXPAND | wxALL, 5);
        sizer->Add(htmlWindow, 1, wxEXPAND | wxALL, 5); // Add the htmlWindow to the sizer

        panel->SetSizer(sizer); // Set the sizer for the panel
    }

    void GetSql(wxString text) {
        string sql = "SELECT content FROM questions WHERE question LIKE \'%" + text.ToStdString() + "%\';";
        rc = sqlite3_exec(db, sql.c_str(), callback, str, &zErrMsg);

        if (rc) {
            htmlWindow->SetPage("<p> failed to open database <p>");
        }
    }

    void OnSearch(wxCommandEvent& event) {
        //make this actually work
        wxString text = searchCtrl->GetValue();
        if (text.length() > 5) {
            str->clear();
            GetSql(text);
            htmlWindow->SetPage(*str);
        }
        else {
            htmlWindow->SetPage("<p> Try a longer keyword <p>");;
        }
    }

    void OnResize(wxSizeEvent& event) {
        sizer->SetMinSize(event.GetSize());
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame("jurgas++");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);