#include "main.h"

wxIMPLEMENT_APP(MyApp);

MyApp::MyApp() {
        /* Linux specific init */
  #ifdef __linux__
        int status = XInitThreads();
  #endif
}

bool MyApp::OnInit()
{
        /* TODO: Check from opencv size of video frame, and supply to make matching window size */
        p_frame = new MyFrame( "Video source", wxPoint(0, 0), wxSize(1024, 768) );
        p_frame->Show( true );

        p_cvWorker = new cvWorker();
        if (p_cvWorker->Create() != wxTHREAD_NO_ERROR) {
                wxExit();
        }
        if (p_cvWorker->Run() != wxTHREAD_NO_ERROR) {
                wxExit();
        }
        return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
        #ifndef __linux__
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        #endif

        /* For blinking red dot :) */
        m_timer = new wxTimer(this, TIMER_ID);
        m_timer->Start(1000);
}

void MyFrame::paintEvent(wxPaintEvent & evt) {
#ifndef __linux__
        /* This stuff perform bad on Linux (stop updating window after while) */
        wxAutoBufferedPaintDC dc(this);
#else
        wxPaintDC dc(this);
#endif

        render(dc);
}

/* Sometimes refresh stalls */
void MyFrame::OnTimer(wxTimerEvent& event)
{
        if (dot_on)
                dot_on = 0;
        else
                dot_on = 1;
}

void MyFrame::render(wxDC& dc) {
        // If mutex not busy - redraw
        if (p_GUImutex.TryLock() == wxMUTEX_NO_ERROR) {
                if (p_wxBmp.IsOk() == true ) {
                        dc.DrawBitmap(p_wxBmp, 0, 0, false );
                        /* Blinking red dot */
                        dc.SetPen(*wxRED_PEN);
                        dc.SetBrush(*wxRED_BRUSH);
                        if (dot_on)
                          dc.DrawCircle(20,20,3);
                }
                p_GUImutex.Unlock();
        } else {
                /* Normally this should not happen? unless your cpu(s) are fully loaded */
                printf("WARNING! Frame drop in GUI\r\n");
        }
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_PAINT(MyFrame::paintEvent)
EVT_TIMER(TIMER_ID, MyFrame::OnTimer)
END_EVENT_TABLE()
