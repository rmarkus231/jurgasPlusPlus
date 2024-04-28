#include <wx/wxprec.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/event.h>
#include <wx/textwrapper.h>

#include "app.h"
#include "tinyXML/tinyxml2.h"
#include <vector>
#include <string>
#include <iostream>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define ID_MENU_HELLO   1 // Menu Item ID
#define ID_NOTEBOOK		2
#define winDefSize wxSize(800,600)

using namespace std;

bool mainApp::OnInit()
{
	// Create main application window
	Frame* frame = new Frame("Jurgas++", wxPoint(500, 200), winDefSize);

	// Display the frame window
	frame->Show(true);

	// Start the event loop
	return true;
}

Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_MENU_HELLO, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenu* menuMode = new wxMenu;
	menuMode->AppendSeparator();

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuMode, "&Mode");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);
	CreateStatusBar();
	//SetStatusText("Jurgas++");

	// Register events for this frame
	Bind(wxEVT_MENU, &Frame::OnHello, this, ID_MENU_HELLO);
	Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &Frame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_SIZE, &Frame::OnResize, this);

	/*
	mainWin = new wxWindow(this, wxID_ANY);
	//mainWin->SetSizer(mainSizer);
	mainWin->SetSize(this->GetSize());

	wxNotebook *mainNotebook = new wxNotebook(mainWin, ID_NOTEBOOK);
	mainNotebook->SetSize(this->GetSize());
	//mainNotebook->SetSizer(mainSizer);
	wxWindow* calc = new wxWindow(mainNotebook, wxID_ANY);
	wxWindow* questions = new wxWindow(mainNotebook, wxID_ANY);

	mainNotebook->AddPage(questions, wxString("Küsimused"),true);
	mainNotebook->AddPage(calc, wxString("Kalkulaator"));
	
	mainWin->SetFocus();
	//mainWin->Layout();
	*/

	panels = new vector<wxPanel*>;
	mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);
	mainSizer->ComputeFittingWindowSize(this);

	panels->push_back(new wxPanel(this,wxID_ANY));
	panels->push_back(new wxPanel(this, wxID_ANY));
	panels->push_back(new wxPanel(this, wxID_ANY));

	for (auto i = 0; i < panels->size(); i++) {
		wxBoxSizer* temp = new wxBoxSizer(wxVERTICAL);
		mainSizer->Add(panels->at(i),1,wxEXPAND | wxALL,10);
		panels->at(i)->SetSizer(temp);
		panels->at(i)->Hide();
	}

	activePanel = panels->at(0);
	wxStaticText* staticText = new wxStaticText(activePanel, wxID_ANY, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	activePanel->GetSizer()->Add(staticText, 1, wxEXPAND | wxALL, 10);
	activePanel->Show();
}

void Frame::makePanels() {

}

void Frame::OnAbout(wxCommandEvent& event)
{
	//wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void Frame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void Frame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void Frame::OnResize(wxSizeEvent& event) {
	if (activePanel) {
		SetStatusText(to_string(mainSizer->GetSize().x) + " " + to_string(mainSizer->GetSize().y));
		mainSizer->SetMinSize(event.GetSize());
		mainSizer->SetSizeHints(activePanel);
	}
}