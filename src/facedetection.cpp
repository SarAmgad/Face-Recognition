#include "facedetection.h"


Mat FaceDetection::detectFaces(const std::string& imagePath, Mat& ROI)
{
    // Load the pre-trained face cascade
    CascadeClassifier faceCascade;
    faceCascade.load("F:\\haarcascade_frontalface_default.xml");

    // Read the image
    Mat image = imread(imagePath);

    // Convert the image to grayscale
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);

    // Perform face detection
    std::vector<Rect> faces;
    faceCascade.detectMultiScale(grayImage, faces);

    int maxH = 0;

    if(faces.size() > 1){
        for (const auto& face : faces)
        {
            if(face.height > maxH){
                maxH = face.height;
            }
        }
        for(const auto& face : faces){
            if(maxH == face.height){
                rectangle(image, face, Scalar(0, 255, 0), 2);
            }
        }
    }else{
        rectangle(image, faces[0], Scalar(0, 255, 0), 2);
    }

    Mat toReturn;
    grayImage.copyTo(toReturn);

    ROI = toReturn(faces[0]);

    resize(ROI, ROI, Size(100,100));

    return image;
}
