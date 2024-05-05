#include <wx/wx.h>
#include <wx/html/htmlwin.h>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {

        wxPanel* panel = new wxPanel(this, wxID_ANY);

        // Create wxHtmlWindow
        wxHtmlWindow* htmlWindow = new wxHtmlWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
        htmlWindow->LoadFile(wxFileName("data.html"));

        // Set up a sizer to manage layout
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(htmlWindow, 1, wxEXPAND | wxALL, 5); // Add the htmlWindow to the sizer

        panel->SetSizer(sizer); // Set the sizer for the panel
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame("wxHtmlWindow Example");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);