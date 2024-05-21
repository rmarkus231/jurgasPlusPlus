#pragma once
#include <string>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "sqlite/sqlite3.h"

#ifndef SQLTHREAD_H
#define SQLTHREAD_H
using namespace std;

struct sqlThread{
	string* query;
	int* count;
	vector<int>* IDs;

	sqlThread();
	~sqlThread();
	static int callback(void*, int, char**, char**);
	void GetSql(int, sqlite3*, int, char*, wxHtmlWindow*);
	int getCount(sqlite3*);
	static int countCallback(void*, int, char**, char**);
	int idByField(sqlite3*,string);
	static int idByFieldCallback(void*, int, char**, char**);
};
#endif // !SQLTHREAD_H