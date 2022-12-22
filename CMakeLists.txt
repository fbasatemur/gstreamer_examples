cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME gstreamer_example)
project(${PROJECT_NAME})


set(HEADERS
        gst_multicast_handler.hpp)

set(SOURCES
        main.cpp)


find_package(PkgConfig REQUIRED)

pkg_search_module(GST REQUIRED 
        gstreamer-1.0>=1.4
        gstreamer-sdp-1.0>=1.4
        gstreamer-app-1.0>=1.4
        gstreamer-video-1.0>=1.4
        )


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(gstreamer PRIVATE ${GTK3_INCLUDE_DIRS} ${GST_INCLUDE_DIRS})
target_link_libraries(gstreamer ${GTK3_LIBRARIES} ${GST_LIBRARIES})