#pragma once
#include <wx/wx.h>
#include "MyFrame.h"
#ifndef MYAPP_H
#define MYAPP_H
using namespace std;

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame("jurgas++");
        frame->Show(true);
        return true;
    };
};
#endif // !MYAPP_H