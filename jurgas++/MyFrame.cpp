#include "MyFrame.h"
#include "sqlThread.h"
#include <thread>

using namespace std;

int threadTask(sqlThread*& sqth, wxString text, sqlite3* db, int rc, char* zErrMsg, wxHtmlWindow*& win) {
    sqth->GetSql(text, db, rc, zErrMsg, win);
    return 0;
};

int countTask(sqlThread*& sqth, sqlite3* db) {
    sqth->getCount(db);
    return 0;
}

int loadTask(sqlThread*& sqth, sqlite3* db, map<int, wxString>* mp) {
    sqth->fillMap(db, mp);
    return 0;
}

MyFrame::MyFrame(const wxString& title)
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

void MyFrame::OnSearch(wxCommandEvent& event) {
    wxString text = searchCtrl->GetValue();
    if (text.length() > 5) {
        sqth = new sqlThread();
        thread th(threadTask, ref(sqth), text, db, rc, zErrMsg, ref(htmlWindow));
        th.join();
        if (sqth->query->length() < 2) {
            htmlWindow->SetPage("<p> Otsitut ei leitud <p>");
        }
        else {
            htmlWindow->SetPage(wxString::FromUTF8(*(sqth->query)));
        }
    }
    else {
        htmlWindow->SetPage("<p> Proovi sisestada rohkem tähti <p>");
    }
    delete sqth;
}

void MyFrame::OnResize(wxSizeEvent& event) {
    sizer->SetMinSize(event.GetSize());
}

void MyFrame::initContent() {
    //SELECT id FROM questions WHERE question like '%joonis%';
    //SELECT content FROM questions;
    sqth = new sqlThread();
    thread th(loadTask, ref(sqth), db, content);
    th.join();
    delete sqth;
}