#include "sqlThread.h"
#include "sqlite/sqlite3.h"
#include <vector>

using namespace std;

sqlThread::sqlThread() {
    query = new string;
    count = new int(0);
    IDs = new vector<int>;
}

sqlThread::~sqlThread() {
    delete IDs;
    delete query;
    delete count;
}

void sqlThread::GetSql(int id, sqlite3* db, int rc, char* zErrMsg, wxHtmlWindow* win) {
    string sql = "SELECT content FROM questions WHERE id IS " + to_string(id) + ";";
    rc = sqlite3_exec(db, sql.c_str(), callback, query, &zErrMsg);
    if (rc) {
        win->SetPage("<p> Ei suutnud andmebaasi avada. <p>");
    }
}

//data field is query from function above
int sqlThread::callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        *(static_cast<string*>(data)) += argv[i];
        *(static_cast<string*>(data)) += "<br>";
    };
    return 0;
}

int sqlThread::countCallback(void *count, int argc, char** argv, char** azColName) {
    *(reinterpret_cast<int*>(count)) = atoi(argv[0]);
    return 0;
}

int sqlThread::getCount(sqlite3* db) {
    string sql = "SELECT max(id) FROM questions;";
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), countCallback, count, &zErrMsg);
    if (rc != SQLITE_OK) {
        return 1;
    }else {
        return 0;
    }
}

int sqlThread::idByField(sqlite3* db, string text) {
    string sql = "SELECT id FROM questions WHERE question LIKE \'%" + text + "%\';";
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), idByFieldCallback, IDs, &zErrMsg);
    if (rc != SQLITE_OK) {
        return 1;
    }
    else {
        return 0;
    }
}

int sqlThread::idByFieldCallback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        (static_cast<vector<int>*>(data))->push_back(stoi(argv[i]));
    };
    return 0;
}