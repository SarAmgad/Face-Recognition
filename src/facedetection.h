#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


using namespace std;
using namespace cv;


class FaceDetection
{
public:
    Mat detectFaces(const std::string& imagePath, Mat& ROI);
};

#endif // FACEDETECTION_H
