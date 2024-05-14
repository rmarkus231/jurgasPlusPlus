#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "sqlite/sqlite3.h"
#include <string.h>
#include <wx/image.h>
#include <thread>

using namespace std;

struct sqlThread {
    //this wasnt actually where the problem was. Off to multithread ui drawing then...
    string* query;
    sqlThread(){
        query = new string;
    };
    static int callback(void* data, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
            /*
            *(static_cast<wxString*>(data)) += wxString(argv[i], wxConvUTF8);
            *(static_cast<wxString*>(data)) += wxString("<br>", wxConvUTF8);
            */
            *(static_cast<string*>(data)) += argv[i];
            *(static_cast<string*>(data)) += "<br>";
        };
        return 0;
    };

    void GetSql(wxString text,sqlite3* db, int rc, char* zErrMsg, wxHtmlWindow* win) {
        string sql = "SELECT content FROM questions WHERE question LIKE \'%" + text.ToStdString() + "%\';";
        rc = sqlite3_exec(db, sql.c_str(), callback, query, &zErrMsg);
        if (rc) {
            win->SetPage("<p> Ei suutnud andmebaasi avada. <p>");
        }
    }
};

int threadTask(sqlThread*& sqth, wxString text, sqlite3* db, int rc, char* zErrMsg, wxHtmlWindow*& win) {
    sqth->GetSql(text, db, rc, zErrMsg, win);
    return 0;
};

struct MyFrame : public wxFrame {
    wxHtmlWindow* htmlWindow;
    wxPanel* panel;
    wxTextCtrl* searchCtrl;
    wxBoxSizer* sizer;

    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;

    

    MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
        rc = sqlite3_open("data.db", &db);

        panel = new wxPanel(this, wxID_ANY);
        searchCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        searchCtrl->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnSearch, this);
        htmlWindow = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);

        wxInitAllImageHandlers();

        //str = new wxString("", wxConvUTF8);

        // Set up a sizer to manage layout
        sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(searchCtrl, 0, wxEXPAND | wxALL, 5);
        sizer->Add(htmlWindow, 1, wxEXPAND | wxALL, 5); // Add the htmlWindow to the sizer

        panel->SetSizer(sizer); // Set the sizer for the panel
    }

    void OnSearch(wxCommandEvent& event) {
        wxString text = searchCtrl->GetValue();
        if (text.length() > 5) {
            sqlThread* sqth = new sqlThread();
            thread th(threadTask, ref(sqth), text, db, rc, zErrMsg, ref(htmlWindow));
            th.join();
            if (sqth->query->length() < 2) {
                htmlWindow->SetPage("<p> Otsitut ei leitud <p>");
            }else {
                htmlWindow->SetPage(wxString::FromUTF8(*(sqth->query)));
            }
        }
        else {
            htmlWindow->SetPage("<p> Proovi sisestada rohkem tähti <p>");
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