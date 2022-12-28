#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace GST_PARAMS{

    static const constexpr int FRAME_WIDTH = 640; 
    static const constexpr int FRAME_HEIGHT = 480;
    static const std::string multicast_ip{"127.0.0.1"};
    static const std::string multicast_port{"5000"};

    // webcam_server_pipeline: gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg,width=1280,height=720,type=video,framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! x264enc tune=zerolatency ! rtph264pay ! udpsink host=127.0.0.1 port=5000 --verbose
    // webcam_client_pipeline: gst-launch-1.0 -vc udpsrc port=5000 close-socket=false multicast-iface=false auto-multicast=true ! application/x-rtp, payload=96 ! rtpjitterbuffer ! rtph264depay ! avdec_h264 ! fpsdisplaysink  sync=false --verbose

//    static std::string webcam_client_pipeline = std::string{"udpsrc multicast-group=" + multicast_ip + " port=" + multicast_port +
//                                                             " ! application/x-rtp, media=video, encoding-name=H264 ! rtpjitterbuffer ! rtph264depay ! h264parse ! avdec_h264 ! xvimagesink name=appsink0"};

    static std::string webcam_client_pipeline = std::string{"udpsrc multicast_group=127.0.0.1 port=5000 auto-multicast=true ! application/x-rtp, payload=96 ! rtpjitterbuffer ! rtph264depay ! avdec_h264 ! appsink name=appsink0"};





    static const std::string video_path{"../video/g_video.mp4"};

    static std::string filesrc_server_pipeline = std::string{"filesrc location=" + video_path +
                                                                " ! qtdemux name=d d. ! h264parse ! rtph264pay config-interval=3 pt=96 mtu=1024"
                                                                " ! udpsink host=" + multicast_ip + " port=" + multicast_port};

    // filesrc_server_pipeline: gst-launch-1.0 filesrc location=../video/g_video.mp4 ! qtdemux name=d d. ! h264parse ! rtph264pay config-interval=3 pt=96 mtu=1024 ! udpsink host=127.0.0.1 port=5000
    // filesrc_client_pipeline ?

    
}

#endif