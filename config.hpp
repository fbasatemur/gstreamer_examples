#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <chrono>
#define USE_WEBCAM true

namespace GST_PARAMS{
    using namespace std::chrono_literals;

    static const constexpr auto SLEEP_TIME = 50ms;

#if USE_WEBCAM
    static int const constexpr FRAME_WIDTH = 1280, FRAME_HEIGHT = 720;
#else   // filesrc is used to read from video
    static int const constexpr FRAME_WIDTH = 640, FRAME_HEIGHT = 360;
#endif

    static const std::string multicast_ip{"127.0.0.1"};
    static const std::string multicast_port{"5000"};
    static const std::string appsink_name{"appsink0"};

    /* Streamer pipelines is below to read from webcam or video
    * webcam_udp_server_pipeline: gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg,width=1280,height=720,type=video,framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! x264enc tune=zerolatency ! rtph264pay ! udpsink name=appsink0 host=127.0.0.1 port=5000 --verbose
    * filesrc_udp_server_pipeline: gst-launch-1.0 filesrc location=video/g_video.mp4 ! qtdemux name=d d. ! h264parse ! rtph264pay config-interval=3 pt=96 mtu=1024 ! udpsink name=appsink0 host=127.0.0.1 port=5000
    */

    static std::string udp_client_pipeline = std::string{"udpsrc multicast-group=" + multicast_ip + " port=" + multicast_port +
                                                         " ! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 "
                                                         "! decodebin ! videoconvert ! video/x-raw,width=" + std::to_string(FRAME_WIDTH) + ",height=" + std::to_string(FRAME_HEIGHT) + ",format=BGR ! videoconvert "
                                                         "! appsink name=" + appsink_name + " emit-signals=true sync=false max-buffers=1 drop=true"};
    
}

#endif