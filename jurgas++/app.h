#pragma once
#ifndef APP_H
#define APP_H
using namespace std;

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
};

class HelloWorld : public wxApp
{
public:
	// Define the initialization method
	// Its called on application start up
	virtual bool OnInit();
};
#endif // !APP_H
