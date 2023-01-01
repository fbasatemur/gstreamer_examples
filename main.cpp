#include "config.hpp"
#include "gst_multicast_handler.hpp"
#define ESC 27

int main()
{
    cv::Mat frame;
    MulticastUDPHandler::StartListen(GST_PARAMS::udp_client_pipeline.c_str());

    while(true)
    {
        frame = MulticastUDPHandler::GetFrame();
        if (cv::waitKey(30) == ESC or frame.empty())
			break;
		imshow("Webcam | ESC", frame);
    }
    
    return 0;
}
