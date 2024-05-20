#include "sqlThread.h"
#include "sqlite/sqlite3.h"
#include <map>

using namespace std;

sqlThread::sqlThread() {
    query = new string;
    count = new int;
}

void sqlThread::GetSql(wxString text, sqlite3* db, int rc, char* zErrMsg, wxHtmlWindow* win) {
    string sql = "SELECT content FROM questions WHERE question LIKE \'%" + text.ToStdString() + "%\';";
    rc = sqlite3_exec(db, sql.c_str(), callback, query, &zErrMsg);
    if (rc) {
        win->SetPage("<p> Ei suutnud andmebaasi avada. <p>");
    }
}

int sqlThread::callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        /*
        *(static_cast<wxString*>(data)) += wxString(argv[i], wxConvUTF8);
        *(static_cast<wxString*>(data)) += wxString("<br>", wxConvUTF8);
        */
        *(static_cast<string*>(data)) += argv[i];
        *(static_cast<string*>(data)) += "<br>";
    };
    return 0;
}

int sqlThread::countCallback(void *count, int argc, char** argv, char** azColName) {
    int* c =  static_cast<int*>(count);
    *c = atoi(argv[0]);
    return 0;
}

int sqlThread::getCount(sqlite3* db) {
    string sql = "SELECT count(*) FROM questions;";
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), callback, &count, &zErrMsg);
    if (rc != SQLITE_OK) {
        return -1;
    }else {
        return 0;
    }
}

int sqlThread::mapCallback(void* mp, int argc, char** argv, char** azColName) {
    map<int, wxString>* content = static_cast<map<int, wxString>*>(mp);
    for (int i = 0; i < argc; i++) {
        //change this to vector or some shit, whys it have to be a map
        content[i] = argv[i];
    }
    return 0;
}

int sqlThread::fillMap(sqlite3* db, map<int, wxString>* mp) {
    return 0;
}