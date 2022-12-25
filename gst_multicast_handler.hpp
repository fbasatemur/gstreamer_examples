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
    
    GstElement *appsink, *gst_pipeline;

    GstSample* sample;
    GstBuffer* buffer;
    GstMapInfo map;

    cv::Size frame_shape;     
    cv::Mat frame;


    GST_Struct(): appsink(nullptr), gst_pipeline(nullptr), sample{nullptr}, buffer{nullptr}{ gst_init(nullptr, nullptr); }

    void SetState(const char* pipeline){
        gst_pipeline = gst_parse_launch(pipeline, NULL);
        g_assert(gst_pipeline);

        gst_element_set_state(gst_pipeline, GST_STATE_PLAYING);
    }
    
    static GstFlowReturn Callback(GstElement* appsink, GST_Struct* gst_self){
        
        g_signal_emit_by_name (appsink, "pull-sample", &gst_self->sample, NULL);

        gst_self->buffer = gst_sample_get_buffer (gst_self->sample);
        GST_CHECK(gst_self->buffer, "get buffer is null");

        gst_buffer_map (gst_self->buffer, &gst_self->map, GST_MAP_READ);
        
        if(gst_self->frame.empty()){

            const GstStructure* info = NULL;
            GstCaps* caps = NULL;

            caps = gst_sample_get_caps (gst_self->sample);
            GST_CHECK(caps, "get caps is null");

            info = gst_caps_get_structure (caps, 0);
            GST_CHECK(info, "get info is null");

            // convert gstreamer data to OpenCV Mat
            gst_structure_get_int (info, "width", &gst_self->frame_shape.width);
            gst_structure_get_int (info, "height", &gst_self->frame_shape.height);

            gst_self->frame = cv::Mat(gst_self->frame_shape, CV_8UC3, (unsigned char*)gst_self->map.data, cv::Mat::AUTO_STEP);
        }
        
        return GST_FLOW_OK;
    }

    void TryAppsink(char* appsink_str, int sink_no = 0){
        try{
            appsink = gst_bin_get_by_name(GST_BIN(gst_pipeline), (std::string(appsink_str) + std::to_string(sink_no)).c_str());
        }catch(std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            TryAppsink(appsink_str, ++sink_no);
        }
    }

    cv::Mat&& GetFrameFromConnection(){

        g_signal_connect(appsink, "new-sample", G_CALLBACK(Callback), this);
        return frame.clone();
    }

    ~GST_Struct(){
        gst_element_set_state (gst_pipeline, GST_STATE_NULL);
        gst_object_unref (gst_pipeline);
        gst_object_unref (appsink);
        if(buffer) gst_buffer_unmap (buffer, &map);
        if(sample) gst_sample_unref (sample);
    }
};


class MulticastUDPHandler{

    friend struct GST_Struct;
    static GST_Struct gst_stream;
public:
    MulticastUDPHandler() = default;

    static void StartListen(const char* pipeline){
        gst_stream.SetState(pipeline);
        gst_stream.TryAppsink("appsink");
    }

    static cv::Mat&& GetFrame(){
        return gst_stream.GetFrameFromConnection();
    }
    
    ~MulticastUDPHandler() = default;
};

#endif // GST_MULTICAST_HANDLER_CPP