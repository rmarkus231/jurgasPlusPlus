#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/thread.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

class HtmlLoaderThread : public wxThread {
public:
    HtmlLoaderThread(wxHtmlWindow* htmlWindow, const wxString& filePath)
        : wxThread(wxTHREAD_DETACHED), m_htmlWindow(htmlWindow), m_filePath(filePath) {}

protected:
    virtual ExitCode Entry() override {
        if (!m_htmlWindow || m_filePath.empty())
            return nullptr;

        // Load and parse HTML content
        wxString htmlContent;
        if (wxFile::Exists(m_filePath)) {
            wxFileInputStream fileStream(m_filePath);
            if (fileStream.IsOk()) {
                wxTextInputStream textStream(fileStream);
                htmlContent = textStream.ReadAll();
            }
        }

        // Notify the main thread that loading is complete
        wxCommandEvent evt(wxEVT_THREAD, wxID_ANY);
        evt.SetString(htmlContent);
        m_htmlWindow->GetEventHandler()->AddPendingEvent(evt);

        return nullptr;
    }

private:
    wxHtmlWindow* m_htmlWindow;
    wxString m_filePath;
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
        m_htmlWindow = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
        m_htmlWindow->SetPage("<p>Loading HTML content...</p>");

        // Start HTML loading thread
        m_loaderThread = new HtmlLoaderThread(m_htmlWindow, "path/to/your/file.html");
        m_loaderThread->Create();
        m_loaderThread->Run();

        Bind(wxEVT_THREAD, &MyFrame::OnThreadEvent, this);
    }

    void OnThreadEvent(wxThreadEvent& evt) {
        wxString htmlContent = evt.GetString();
        if (!htmlContent.empty()) {
            m_htmlWindow->SetPage(htmlContent);
        }
    }

private:
    wxHtmlWindow* m_htmlWindow;
    HtmlLoaderThread* m_loaderThread;
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame("HTML Loader Thread Example");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
