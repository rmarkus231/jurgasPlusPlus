#pragma once
#include <vector>
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

    vector<string*>* content;

    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc;
    MyFrame(const wxString&);
    void OnSearch(wxCommandEvent&);
    void OnResize(wxSizeEvent&);
    void initContent();
};

int threadTaskID(sqlThread*&, string, sqlite3*);
int countTask(sqlThread*&,sqlite3*);
int threadTask(sqlThread*&, int, sqlite3*, int, char*, wxHtmlWindow*&);
#endif //MYFRAME_H