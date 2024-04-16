#include <wx/wxprec.h>
#include "app.h"
#include <vector>
#include <string>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define ID_MENU_HELLO   1 // Menu Item ID

using namespace std;

bool HelloWorld::OnInit()
{
	// Create main application window
	Frame* frame = new Frame("Hello World", wxPoint(500, 500), wxSize(300, 300));

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

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");

	// Register events for this frame
	Bind(wxEVT_MENU, &Frame::OnHello, this, ID_MENU_HELLO);
	Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &Frame::OnExit, this, wxID_EXIT);

	vector<wxButton*>* buttonVector = new vector<wxButton*>;
	wxWindow* window = new wxWindow(this, wxID_ANY);
	
	for (auto i = 0; i < 5; i++) {
		wxButton* btn = new wxButton(window, wxID_ANY);
		wxString btnStr(to_string(i));
		btn->SetLabel(btnStr);
		btn->SetPosition(wxPoint(0, 20 * i));
		buttonVector->push_back(btn);
	}
	
	window->SetFocus();

}

void Frame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

// Event Handler for clicking the Hello Menu item
// Naviaget to File > Hello in the GUI app
void Frame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

// Event handler on clicking the Exit Menu item
// Navigate to File > Exit in the GUI app
void Frame::OnExit(wxCommandEvent& event)
{
	Close(true);
}