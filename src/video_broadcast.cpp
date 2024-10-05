#include "video_broadcast.hpp"

cv::VideoCapture video_cap;
static bool video_restart_flag = true;

void VideoSet() {
    if (video_restart_flag) {
        video_cap.open("../1.mp4");
        video_restart_flag = false;
    }
    if (!video_cap.isOpened()) {
        printf("failed to open!\n");
    }
    return;
}

void VideoBroadcast() {

    cv::Mat frame;
    video_cap >> frame;
    if (frame.empty()) {
        video_cap.release();
        video_restart_flag = true;
        return;
    }
    imshow("1", frame);
    cv::waitKey(25);
    return;
}
