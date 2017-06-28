#include "cvthread.h"
#include "main.h"
#include "opencv2/opencv.hpp"
using namespace cv;
//using namespace std;


cvWorker::cvWorker(void) : wxThread( wxTHREAD_DETACHED ) {
        return;
}

cvWorker::~cvWorker() {
}

void cvWorker::OnExit() {
}

void *cvWorker::Entry(){
        Mat frame, converted_frame;
        wxImage p_wxImg;
        VideoCapture cap(1);

        if(!cap.isOpened()) {
                // TODO: Signal error and die
                return NULL;
        }
        while(1) {
                bool bSuccess = cap.read(frame);
                if (!bSuccess) {
                        continue;
                }
                if( frame.empty() ) {
                        continue;
                }
                /* OpenCV is BGR, we need RGB */
                #if CV_VERSION_MAJOR < 3
                cvtColor(frame, converted_frame, CV_BGR2RGB, 0);
                p_wxImg = wxImage(converted_frame.cols, converted_frame.rows, converted_frame.data, true);
                #else
                cvtColor(frame, converted_frame, CV_BGR2RGB);
                p_wxImg = wxImage(converted_frame.cols, converted_frame.rows, converted_frame.data, true);
                #endif
                // If mutex not busy, change bitmap
                if (wxGetApp().p_frame->p_GUImutex.TryLock() == wxMUTEX_NO_ERROR) {
                        wxGetApp().p_frame->p_wxBmp = wxBitmap(p_wxImg, -1);
                        wxGetApp().p_frame->p_GUImutex.Unlock();
                        wxGetApp().p_frame->Refresh();
                        /* If we don't call update, it might stall after while */
                        wxGetApp().p_frame->Update();
                } else {
                        /* Normally this should not happen? unless your cpu(s) are fully loaded */
                        printf("WARNING! Frame drop in OpenCV thread\r\n");
                }
        }
}
