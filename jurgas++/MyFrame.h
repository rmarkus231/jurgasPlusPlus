#pragma once
#include <map>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "sqlite/sqlite3.h"
#include "sqlThread.h"

#ifndef MYFRAME_H
#define MYFRAME_H

using namespace std;

struct MyFrame : public wxFrame {
    wxHtmlWindow* htmlWindow;
    wxPanel* panel;
    wxTextCtrl* searchCtrl;
    wxBoxSizer* sizer;
    sqlThread* sqth;

    map<int, wxString>* content;

    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;
    MyFrame(const wxString&);
    void OnSearch(wxCommandEvent&);
    void OnResize(wxSizeEvent&);
    void initContent();
};

int loadTask(sqlThread*&, sqlite3*, map<int, wxString>*);
int countTask(sqlThread*&,sqlite3*);
int threadTask(sqlThread*&, wxString, sqlite3*, int, char*, wxHtmlWindow*&);
#endif //MYFRAME_H