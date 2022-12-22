#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

namespace GST_PARAMS{

    static const constexpr int FRAME_WIDTH = 640; 
    static const constexpr int FRAME_HEIGHT = 480;
    static std::string webcam2appsink_pipeline = std::string{"v4l2src device=/dev/video0 ! video/x-raw, format=RGB, "
                                                                "width=" + std::to_string(FRAME_WIDTH) + ", height=" + std::to_string(FRAME_HEIGHT) + 
                                                                ", pixel-aspect-ratio=1/1, framerate=30/1 ! videoconvert ! appsink"};


}

#endif