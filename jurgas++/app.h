#pragma once
#include <vector>
#ifndef APP_H
#define APP_H
using namespace std;

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	wxBoxSizer* mainSizer;
	vector<wxPanel*>* panels;
	wxPanel* activePanel;

	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnResize(wxSizeEvent& event);
	
	void makePanels();
};

class mainApp : public wxApp
{
public:
	// Define the initialization method
	// Its called on application start up
	virtual bool OnInit();
};
#endif // !APP_H
