# gstreamer_examples

## UDP Multicast Streamer & Receiver
The video stream is multicasted through a Gstreamer pipeline, received by a client pipeline, and each frame is saved to an OpenCV Mat object.

To get the output from your webcam, we should see what kind of resolution and encode the hardware can provide us.
We can learn with below command:
```c
v4l2-ctl --list-formats-ext
```
Or, if you want to get from a specif device:
```c
v4l2-ctl --list-formats-ext --device=0
```

![device_info](https://github.com/fbasatemur/gstreamer_example/blob/main/video/device_info.png)

If I want use to 1280x720 display resolution and 30 FPS.  

Streamer -- webcam pipeline:
```c
gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg,width=1280,height=720,type=video,framerate=30/1 ! jpegdec ! videoscale ! videoconvert ! x264enc tune=zerolatency ! rtph264pay ! udpsink name=appsink0 host=127.0.0.1 port=5600 --verbose

```

If I want use video file with 640 x 360 resolution. 

Streamer -- file or video source pipeline:
```c
gst-launch-1.0 filesrc location=video/g_video.mp4 ! qtdemux name=d d. ! h264parse ! rtph264pay config-interval=3 pt=96 mtu=1024 ! udpsink name=appsink0 host=127.0.0.1 port=5600

```

Client pipeline for application is below.

Receiver pipeline:
```c
gst-launch-1.0 udpsrc multicast-group=127.0.0.1 port=5600 ! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 ! decodebin ! videoconvert ! video/x-raw,width=1280,height=720,format=BGR ! videoconvert ! appsink name=appsink0 emit-signals=true sync=false max-buffers=1 drop=true

```


