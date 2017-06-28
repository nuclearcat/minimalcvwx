#ifndef _MAIN_H
#define _MAIN_H
#include "cvthread.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/dcbuffer.h>
#endif
#ifdef __linux__
#include <X11/Xlib.h>
#endif

class MyFrame;

enum{
    TIMER_ID = wxID_HIGHEST + 1
};

class MyApp: public wxApp
{
public:
    MyApp(void);
    virtual bool OnInit();
    MyFrame *p_frame;
    cvWorker *p_cvWorker;
};

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void paintEvent(wxPaintEvent & evt);
    void render(wxDC& dc);
    void OnTimer(wxTimerEvent& event);

    wxTimer *m_timer;
    wxMutex p_GUImutex;
    wxBitmap p_wxBmp;
    int dot_on;
private:

DECLARE_EVENT_TABLE()
};

DECLARE_APP(MyApp)
#endif
