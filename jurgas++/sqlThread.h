#pragma once
#include <string>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "sqlite/sqlite3.h"
#include <map>

#ifndef SQLTHREAD_H
#define SQLTHREAD_H
using namespace std;

struct sqlThread{
	string* query;
	int* count;

	sqlThread();
	static int callback(void*, int, char**, char**);
	void GetSql(wxString, sqlite3*, int, char*, wxHtmlWindow*);
	int getCount(sqlite3*);
	static int countCallback(void*, int, char**, char**);
	int fillMap(sqlite3*,map<int, wxString>*);
	static int mapCallback(void*, int, char**, char**);
};
#endif // !SQLTHREAD_H