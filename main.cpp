#include "config.hpp"
#include "gst_multicast_receiver.hpp"
#define ESC 27

int main()
{
    cv::Mat frame;
    UDPMulticastReceiver::StartListen(GST_PARAMS::udp_client_pipeline.c_str());

    while(true)
    {
        frame = UDPMulticastReceiver::GetFrame();
        if (cv::waitKey(30) == ESC or frame.empty())
			break;
		imshow("Webcam | ESC", frame);
    }
    
    return 0;
}
