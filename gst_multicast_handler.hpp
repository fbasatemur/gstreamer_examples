#ifndef GST_MULTICAST_HANDLER_CPP
#define GST_MULTICAST_HANDLER_CPP

#include "config.hpp"
#include <string>
#include <gstreamer-1.0/gst/gst.h>
#include <iostream>
#include <opencv2/opencv.hpp>


#define GST_CHECK(ptr, msg) {   \
    if(not ptr){                \
        std::cerr << msg;       \
        return GST_FLOW_ERROR;  \
    }                           \
}

struct GST_Struct{
    
    GstElement *_appsink, *_gst_pipeline;

    GstSample* _sample;
    GstBuffer* _buffer;
    GstMapInfo _map;

    cv::Size _frame_shape;     
    cv::Mat _frame;


    GST_Struct(): _appsink(nullptr), _gst_pipeline(nullptr), _sample{nullptr}, _buffer{nullptr}{ gst_init(nullptr, nullptr); }

    void SetState(const char* pipeline){
        _gst_pipeline = gst_parse_launch(pipeline, NULL);
        g_assert(_gst_pipeline);

        gst_element_set_state(_gst_pipeline, GST_STATE_PLAYING);
    }
    
    GstFlowReturn Callback(GstElement* appsink, gpointer unused){
        
        g_signal_emit_by_name (_appsink, "pull-sample", &_sample, NULL);

        _buffer = gst_sample_get_buffer (_sample);
        GST_CHECK(_buffer, "get buffer is null");

        gst_buffer_map (_buffer, &_map, GST_MAP_READ);
        
        if(_frame.empty()){

            const GstStructure* info = NULL;
            GstCaps* caps = NULL;

            caps = gst_sample_get_caps (_sample);
            GST_CHECK(caps, "get caps is null");

            info = gst_caps_get_structure (caps, 0);
            GST_CHECK(info, "get info is null");

            // convert gstreamer data to OpenCV Mat
            gst_structure_get_int (info, "width", &_frame_shape.width);
            gst_structure_get_int (info, "height", &_frame_shape.height);

            _frame = cv::Mat(_frame_shape, CV_8UC3, (unsigned char*)_map.data, cv::Mat::AUTO_STEP);
        }
        
        return GST_FLOW_OK;
    }

    void TryAppsink(char* appsink_str, int sink_no = 0){
        try{
            _appsink = gst_bin_get_by_name(GST_BIN(_gst_pipeline), (std::string(appsink_str) + std::to_string(sink_no)).c_str());
        }catch(std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            TryAppsink(appsink_str, ++sink_no);
        }
    }

    cv::Mat&& GetFrameFromConnection(){

        g_signal_connect(_appsink, "new-sample", G_CALLBACK(Callback), NULL);
        return _frame.clone();
    }

    ~GST_Struct(){
        gst_element_set_state (_gst_pipeline, GST_STATE_NULL);
        gst_object_unref (_gst_pipeline);
        gst_object_unref (_appsink);
        if(_buffer) gst_buffer_unmap (_buffer, &_map);
        if(_sample) gst_sample_unref (_sample);
    }
};


class MulticastUDPHandler{

    friend struct GST_Struct;
    GST_Struct _gst_stream;

public:
    MulticastUDPHandler() = default;

    void StartListen(){
        _gst_stream.SetState(GST_PARAMS::webcam2appsink_pipeline.c_str());
        _gst_stream.TryAppsink("appsink");
    }

    cv::Mat&& GetFrame(){
        return _gst_stream.GetFrameFromConnection();
    }
    
    ~MulticastUDPHandler() = default;
};

#endif // GST_MULTICAST_HANDLER_CPP