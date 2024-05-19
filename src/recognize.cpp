#include "recognize.h"
#include "traindata.h"
#include <numeric>
#include <algorithm>
#include <iterator>
#include<vector>


void Recognize::prepareFace(Mat test)
{
    test.convertTo(test, CV_32FC1);
    test.reshape(0, test.rows*test.cols).copyTo(testVec);
}

void Recognize::projectFace(Mat testVector, Mat avgVec, Mat eigenVec){
    Mat tmpData;
    subtract(testVector, avgVec, tmpData);
    testPrjFace = eigenVec * tmpData;
}

//Find the closet Euclidean Distance between input and database
void Recognize::recognizeTestimg(Mat test, Mat faces, vector<string>& loadedFacesID)
{
    Mat src2 = test;

    for (int i =0; i < loadedFacesID.size(); i++) {
        Mat src1 = faces.col(i);

        double dist = norm(src1, src2, NORM_L2);
        if (dist < closetFaceDist) {
            closetFaceDist = dist;
            closetFaceID = loadedFacesID[i];
        }
    }
}

string Recognize::getID(Mat& img, Mat avgVec, Mat eigenVec, Mat faces, vector<string>& loadedFacesID){
    prepareFace(img);
    projectFace(testVec, avgVec, eigenVec);
    recognizeTestimg(testPrjFace, faces, loadedFacesID);
    return closetFaceID;
}

Mat Recognize::plot_Roc(const vector<int>& true_labels, const vector<double>& predicted_scores){
    vector<double> fpr;
    vector<double> tpr;
    vector<int> idx(predicted_scores.size());
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int i, int j) { return predicted_scores[i] > predicted_scores[j]; });

    int n_positives = count(true_labels.begin(), true_labels.end(), 1);
    int n_negatives = count(true_labels.begin(), true_labels.end(), 0);
    int tp = 0, fp = 0, tn = n_negatives, fn = n_positives;

    double prev_score = -1.0;
    for (int i = 0; i < idx.size(); i++) {
        int j = idx[i];
        if (predicted_scores[j] != prev_score) {
            fpr.push_back((double)fp / n_negatives);
            tpr.push_back((double)tp / n_positives);
            prev_score = predicted_scores[j];
        }
        if (true_labels[j] == 1) {
            tp++;
            fn--;
        }
        else {
            fp++;
            tn--;
        }
    }
    fpr.push_back((double)fp / n_negatives);
    tpr.push_back((double)tp / n_positives);

    // Plot ROC curve
    Mat img = Mat(480, 640, CV_8UC3, Scalar(249, 249, 249));
    line(img, Point(0, 480), Point(640, 480), Scalar(0, 0, 0), 2);
    line(img, Point(0, 0), Point(0, 480), Scalar(0, 0, 0), 2);


    double x_min = *min_element(fpr.begin(), fpr.end());
    double x_max = *max_element(fpr.begin(), fpr.end());
    double y_min = *min_element(tpr.begin(), tpr.end());
    double y_max = *max_element(tpr.begin(), tpr.end());
    double x_range = x_max - x_min;
    double y_range = y_max - y_min;
    double x_scale = (img.cols) / x_range;
    double y_scale = (img.rows) / y_range;

    vector<Point> pts(2*fpr.size() - 2);

    for (int i = 0; i < (fpr.size()-1); i++) {
        pts[2*i] = Point((fpr[i] - x_min) * x_scale, img.rows -( (tpr[i] - y_min)* y_scale));
        pts[(2*i)+1] = Point((fpr[i+1] - x_min) * x_scale, img.rows - ((tpr[i] - y_min) * y_scale));
    }

    // Draw the lines between each pair of adjacent points
    for (int i = 0; i < pts.size() - 1 ; i++) {
        line(img, pts[i], pts[i + 1], Scalar(225, 0, 0), 5);
        circle(img, pts[i], 5, Scalar(128, 128, 128), -2);
    }

    return img;
}


Mat Recognize::performance(int start, int end,int k) {

    string folderTrained = "..\\Training\\";
    String folderPatternTrained = folderTrained + "/*.png";
    vector<string> trainedFileNames;
    glob(folderPatternTrained, trainedFileNames);
    vector<string> loadedFaceID;
    vector<int>lables(30);

    TrainData read;
    Mat faces = read.readFaces(30, loadedFaceID);
    for(int i = 0 ; i < loadedFaceID.size(); i++){
        int str_num = stoi(loadedFaceID[i]);
        if(str_num >= start && str_num <= end){
            lables[i] = 1;
        }
        else{
            lables[i] = 0;
        }
    }

    vector<double> scores;
    for (int i = 0; i < loadedFaceID.size() ;i++) {
        vector<double> columnDistances(30);
            for (int j = 0; j < faces.cols; ++j) {
                Mat col1 = faces.col(i);
                Mat col2 = faces.col(j);
                double distance = norm(col1, col2,NORM_L2);
                columnDistances[j] = distance;
            }
            vector<int> minimums(k);
            for (int i = 0; i < k ; i++) {
                vector<double>::iterator result = min_element(columnDistances.begin(), columnDistances.end());
                int min = distance(columnDistances.begin(), result);
                minimums[i] = min;
                columnDistances[min] = INT_MAX;
            }
            double percentage = 0.0;
            for(int i = 0 ; i< k ; i++){
                percentage += lables[minimums[i]];
            }

            percentage /= k;
            scores.push_back(percentage);
    }

    return plot_Roc(lables,scores);
}

