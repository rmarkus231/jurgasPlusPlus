#include <wx/wxprec.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/event.h>
#include <wx/textwrapper.h>
#include <wx/html/htmlwin.h>

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
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	// Create wxHtmlWindow
	wxHtmlWindow* htmlWindow = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
	htmlWindow->LoadPage(wxString("/data.html"));

	// Set up a sizer to manage layout
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(htmlWindow, 1, wxEXPAND | wxALL, 5); // Add the htmlWindow to the sizer

	panel->SetSizer(sizer); // Set the sizer for the panel
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
		//mainSizer->SetSizeHints(htmlWindow);
	}
}