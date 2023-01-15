cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME gstreamer_example)
project(${PROJECT_NAME})


set(HEADERS
        config.hpp
        gst_multicast_receiver.hpp)

set(SOURCES
        main.cpp)


find_package(PkgConfig)

pkg_search_module(GST REQUIRED 
        gstreamer-1.0>=1.4
        gstreamer-sdp-1.0>=1.4
        gstreamer-app-1.0>=1.4
        gstreamer-video-1.0>=1.4
        )


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${GST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GST_LIBRARIES} ${OpenCV_LIBS})