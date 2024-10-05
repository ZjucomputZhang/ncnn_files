#ifndef YOLOV5_LITE_HPP
#define YOLOV5_LITE_HPP

#include "layer.h"
#include "net.h"

#if defined(USE_NCNN_SIMPLEOCV)
#include "simpleocv.h"
#else
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif
#include <float.h>
#include <stdio.h>
#include <vector>
#include "time.h"

int GetDetectNum();
int GetDetectLabel();
void DetectSet(int w, int h);
bool Yolov5liteDetect();

#endif
