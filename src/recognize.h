#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;



class Recognize
{
public:
    void prepareFace(Mat test);
    void projectFace(Mat testVec, Mat avgVec, Mat eigenVec);
    void recognizeTestimg(Mat test, Mat faces, vector<string>& loadedFacesID);
    string getID(Mat& img, Mat avgVec, Mat eigenVec, Mat faces, vector<string>& loadedFacesID);
    Mat performance(int start, int end, int k);
    Mat plot_Roc(const std::vector<int>& true_labels, const std::vector<double>& predicted_scores);


    Mat testVec;
    Mat testPrjFace;
    string closetFaceID ="0";
    double closetFaceDist = INT_MAX;

};

#endif // RECOGNIZE_H
