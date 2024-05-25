#include "MyFrame.h"
#include "sqlThread.h"
#include <thread>
#include <fstream>
#include <wx/utils.h>

using namespace std;

int threadTask(sqlThread*& sqth, int id, sqlite3* db, int rc, char* zErrMsg, wxHtmlWindow*& win) {
    sqth->GetSql(id, db, rc, zErrMsg, win);
    return 0;
};

int threadTaskID(sqlThread*& sqth, string text, sqlite3* db) {
    sqth->idByField(db, text);
    return 0;
};

int countTask(sqlThread*& sqth, sqlite3* db) {
    sqth->getCount(db);
    return 0;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    rc = sqlite3_open("data.db", &db);

    panel = new wxPanel(this, wxID_ANY);
    searchCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    searchCtrl->Bind(wxEVT_TEXT_ENTER, &MyFrame::OnSearch, this);
    htmlWindow = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);

    //str = new wxString("", wxConvUTF8);

    // Set up a sizer to manage layout
    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(searchCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(htmlWindow, 1, wxEXPAND | wxALL, 5); // Add the htmlWindow to the sizer

    panel->SetSizer(sizer); // Set the sizer for the panel
    htmlWindow->AppendToPage("<p> Loading DB into memory <p>\n");
    initContent();
    htmlWindow->AppendToPage("<p> DB loaded <p>\n");
}

void MyFrame::OnSearch(wxCommandEvent& event) {
  
    wxString text = searchCtrl->GetValue();
    if (text.length() > 5) {
        sqlThread* sqth = new sqlThread();
        thread th(threadTaskID, ref(sqth), text.ToStdString(), db);
        th.join();
        if (sqth->IDs->empty()) {
            htmlWindow->AppendToPage("<p> Otsitut ei leitud <p>\n");
        }
        else {
            vector<int>::iterator it;
            ofstream htmlFile;
            string file = "temp.html";
            htmlFile.open(file, ios::out);
            htmlFile << "<!DOCTYPE html>\n<html lang = \"en\">\n<head>\n<meta charset = \"UTF-8\" />\n<title>Jurgas++</title>\n</head>\n<body>";
            for (it = sqth->IDs->begin(); it != sqth->IDs->end();it++){
                try {
                    htmlFile << *(content->at(*it));
                }
                catch (exception& e) {
                    auto error = e;
                }
            }
            htmlFile << "</body>\n</html>";
            htmlFile.close();

            //fuck this embeded html loading, slow as SHIT, html file with text compiled in ms
            // page loaded in MINUTES
            //htmlWindow->LoadFile(wxString(file));
            try{ 
                wxLaunchDefaultBrowser(wxString(file)); 
            }
            catch (exception& e) {
                //sqth->IDs containing out of range values for questions
                auto error = e;
                htmlWindow->AppendToPage(e.what());
            }
        }
        delete sqth;
    }
    else {
        htmlWindow->AppendToPage("<p> Proovi sisestada rohkem tähti <p>\n");
    }
}

void MyFrame::OnResize(wxSizeEvent& event) {
    sizer->SetMinSize(event.GetSize());
}

void MyFrame::initContent() {
    //SELECT id FROM questions WHERE question like '%joonis%';
    //SELECT content FROM questions;
    sqlThread* sqth = new sqlThread();
    thread th(countTask, ref(sqth), db);
    th.join();
    int c = *(sqth->count);
    delete sqth;

    content = new vector<string*>(c+1);

    //gonna use n threads to load stuff into memory
    //no idea if this is the most efficient way but i dont want to create ~3200 threads at once, 
    //seems like a bad idea
    int threads = 20;
    //declare variables for inner loops so i can use them in VS memory view incase of error
    int j, k, l, m, n;

    //forgot to add the division part, you try debugging multithreaded seqfaults
    for (int i = 0; i < (c - (c % threads)) / threads; i++) {
        try {
            vector<sqlThread*> sqlThreadVec;
            vector<thread> threadVec;
            for (j = 0; j < threads; j++) {
                sqlThreadVec.push_back(new sqlThread());
            }
            for (k = 0; k < threads; k++) {
                threadVec.push_back(thread(threadTask, ref(sqlThreadVec[k]), i * threads + k, db, rc, zErrMsg, ref(htmlWindow)));
            }
            for (l = 0; l < threads; l++) {
                threadVec[l].join();
            }
            for (m = 0; m < threads; m++) {
                content->at(i * threads + m) = new string(*(sqlThreadVec[m]->query));
            }
            for (n = 0; n < threads; n++) {
                //remember to clean up after yourself
                delete sqlThreadVec[n];
            }
        }
        catch (exception& e) {
            auto error = e;
            htmlWindow->AppendToPage(e.what());
        }
    }
    //deal with rest of the questions being loaded incase its not
    //divisible by thread count
    for (int i = c - c % threads; i < c; i++) {
        sqlThread* sqt = new sqlThread();
        thread th(threadTask, ref(sqt), i, db, rc, zErrMsg, ref(htmlWindow));
        th.join();
        content->at(i) = sqt->query;
        delete sqt;
    }
}