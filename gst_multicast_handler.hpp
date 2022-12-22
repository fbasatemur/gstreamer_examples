#ifndef GST_MULTICAST_HANDLER_CPP
#define GST_MULTICAST_HANDLER_CPP

#include "config.hpp"
#include <string>
#include <gstreamer-1.0/gst/gst.h>
#include <iostream>

struct GST_Struct{
    GstElement *_stream_sink = nullptr;
    GstElement *_gst_pipeline = nullptr;

    GST_Struct(): _stream_sink(nullptr), _gst_pipeline(nullptr){}

    void Set(const char* pipeline){
        _gst_pipeline = gst_parse_launch(pipeline, NULL);
        g_assert(_gst_pipeline);

        gst_element_set_state (_gst_pipeline, GST_STATE_PLAYING);
    }

    void TryAppsink(char* appsink_str, int sink_num = 0){
        try{
            
        }catch(std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            TryAppsink(appsink_str, ++sink_num);
        }
    }
    
    ~GST_Struct(){
        gst_element_set_state (_gst_pipeline, GST_STATE_NULL);
        gst_object_unref (_gst_pipeline);
        gst_object_unref (_stream_sink);
    }
};


class GST_UDP_Multicast_Handler{
    GST_Struct _gst_stream;
    friend struct GST_Struct;

public:
    GST_UDP_Multicast_Handler()
    {
        gst_init(nullptr, nullptr);
    }

    void Handler_Start(){
        _gst_stream.Set(GST_PARAMS::webcam2appsink_pipeline.c_str());
        _gst_stream.TryAppsink("appsink");
;
    }
    // void Callback(GstElement*instance, , ){}
    
    ~GST_UDP_Multicast_Handler(){
    
    } 

};

#endif // GST_MULTICAST_HANDLER_CPP