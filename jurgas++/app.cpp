#include <wx/wxprec.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/event.h>

#include "app.h"
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

	//mainSizer = new wxGridSizer(2,this->GetSize());

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
}

void Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
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
	SetStatusText(to_string(event.GetSize().x)+" "+ to_string(event.GetSize().y) +"\t"+mainWin->GetWindowChild(ID_NOTEBOOK)->GetName());
}