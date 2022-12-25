#include <iostream>
#include "config.hpp"
#include "gst_multicast_handler.hpp"

int main()
{
    cv::Mat frame;
    MulticastUDPHandler::StartListen(GST_PARAMS::webcam_client_pipeline.c_str());

    while(true)
    {
        frame = MulticastUDPHandler::GetFrame();
        if (cv::waitKey(30) == 27 || frame.empty())		// esc basilana kadar ya da matris bos ise
			break;
		imshow("webcam", frame);
    }
    
    return 0;
}
