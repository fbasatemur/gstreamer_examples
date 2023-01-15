#ifndef GST_MULTICAST_RECEIVER_CPP
#define GST_MULTICAST_RECEIVER_CPP

#include <gstreamer-1.0/gst/gst.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <string>
#include "config.hpp"


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

    bool frame_initalized;
    cv::Size frame_shape;
    cv::Mat frame;

    GST_Struct(){
        appsink = nullptr;
        gst_pipeline = nullptr;
        sample = nullptr;
        buffer = nullptr;
        frame_initalized = false;
        gst_init(nullptr, nullptr);
    }

    void StartPipeline(const char* pipeline){
        gst_pipeline = gst_parse_launch(pipeline, nullptr);
        g_assert(gst_pipeline);

        gst_element_set_state(gst_pipeline, GST_STATE_PLAYING);
    }

    GstFlowReturn SetFrameInfo(){
        GstStructure* info {nullptr};
        GstCaps* caps {nullptr};

        caps = gst_sample_get_caps (sample);
        GST_CHECK(caps, "get caps is null");

        info = gst_caps_get_structure (caps, 0);
        GST_CHECK(info, "get info is null");

        gst_structure_get_int (info, "width", &frame_shape.width);
        gst_structure_get_int (info, "height", &frame_shape.height);
        return GST_FLOW_OK;
    }

    void ReadGSTMap2Mat(){
        gst_buffer_map(buffer, &map, GST_MAP_READ);
        frame = cv::Mat(frame_shape, CV_8UC3, (char *)map.data, cv::Mat::AUTO_STEP);
    }

    static GstFlowReturn Callback(GstElement* appsink, GST_Struct* gst_self){

        g_signal_emit_by_name (appsink, "pull-sample", &gst_self->sample, NULL);

        gst_self->buffer = gst_sample_get_buffer (gst_self->sample);
        GST_CHECK(gst_self->buffer, "get buffer is null");

        if(not gst_self->frame_initalized)
        {
            if(gst_self->SetFrameInfo() != GST_FLOW_OK)
                return GST_FLOW_ERROR;

            gst_self->frame_initalized = true;
        }

        gst_self->ReadGSTMap2Mat();

        gst_buffer_unmap(gst_self->buffer, &gst_self->map);
        gst_sample_unref(gst_self->sample);

        return GST_FLOW_OK;
    }

    void ConnectAppsink(const char* appsink_str){
        appsink = gst_bin_get_by_name(GST_BIN(gst_pipeline), (const gchar*)(appsink_str));
        if (not appsink){
            std::cerr << "GST appsink is NULL !\n";
            exit(0);
        }
    }

    void StartCallBack(){
        g_object_set (appsink, "emit-signals", TRUE, NULL);
        g_signal_connect(appsink, "new-sample", G_CALLBACK(Callback), this);
    }

    const cv::Mat* GetFrameFromConnection(){
        while (frame.empty()){
            std::cerr << "Frame is not exist !\n";
            std::this_thread::sleep_for(GST_PARAMS::SLEEP_TIME);
        }

        return &frame;
    }

    ~GST_Struct(){
        if(gst_pipeline) {
            gst_element_set_state (gst_pipeline, GST_STATE_NULL);
            gst_object_unref (gst_pipeline);
        }
        if(appsink) gst_object_unref (appsink);
    }
};


class UDPMulticastReceiver{

    friend struct GST_Struct;
    inline static GST_Struct gst_stream;
public:
    UDPMulticastReceiver() = default;

    static void StartListen(const char* pipeline){
        gst_stream.StartPipeline(pipeline);
        gst_stream.ConnectAppsink(GST_PARAMS::appsink_name.c_str());
        gst_stream.StartCallBack();
    }

    static cv::Mat GetFrame(){
        return gst_stream.GetFrameFromConnection()->clone();
    }
    
    ~UDPMulticastReceiver() = default;
};

#endif // GST_MULTICAST_RECEIVER_CPP