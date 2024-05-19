#ifndef DoPCA_H
#define DoPCA_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "facedetection.h"

using namespace std;
using namespace cv;



class DoPCA
{
public:
    Mat getEigenVector(Mat cov, Mat subFaces);
    void prePCA();
    Mat applyPCA();

    int imgSize;
    int imgRows;
    Mat features;
    Mat avgFace;
    Mat subFaces;
    Mat cov;
    int col = 30;  //no. of training images
};

#endif // PCA_H
