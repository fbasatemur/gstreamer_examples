#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <chrono>

namespace GST_PARAMS{
    using namespace std::chrono_literals;

    static const constexpr auto SLEEP_TIME = 20ms;
    static const constexpr int FRAME_WIDTH = 1280;
    static const constexpr int FRAME_HEIGHT = 720;
    static const std::string multicast_ip{"127.0.0.1"};
    static const std::string multicast_port{"5000"};

    // webcam_server_pipeline: gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg,width=1280,height=720,type=video,framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! x264enc tune=zerolatency ! rtph264pay ! udpsink host=127.0.0.1 port=5000 --verbose

    static std::string webcam_client_pipeline = std::string{"udpsrc multicast-group=" + multicast_ip + " port=" + multicast_port +
                                                            " ! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 "
                                                            "! decodebin ! videoconvert ! video/x-raw,width=" + std::to_string(FRAME_WIDTH) + ",height=" + std::to_string(FRAME_HEIGHT) + ",format=BGR ! videoconvert "
                                                            "! appsink name=appsink0 emit-signals=true sync=false max-buffers=1 drop=true"};



    static const std::string video_path{"../video/g_video.mp4"};

    static std::string filesrc_server_pipeline = std::string{"filesrc location=" + video_path +
                                                                " ! qtdemux name=d d. ! h264parse ! rtph264pay config-interval=3 pt=96 mtu=1024"
                                                                " ! udpsink host=" + multicast_ip + " port=" + multicast_port};

    // filesrc_server_pipeline: gst-launch-1.0 filesrc location=../video/g_video.mp4 ! qtdemux name=d d. ! h264parse ! rtph264pay config-interval=3 pt=96 mtu=1024 ! udpsink host=127.0.0.1 port=5000
    // filesrc_client_pipeline ?

    
}

#endif