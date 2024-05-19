#include "traindata.h"

void TrainData::WriteData(){
    DoPCA PCA;
    PCA.prePCA();
    int facesNumber = PCA.features.cols;
    trainFaceseign.create(facesNumber, facesNumber, CV_32FC1);
    Mat faces = PCA.features;
    Mat avg = PCA.avgFace;
    Mat eigenVector = PCA.applyPCA();

    for(int i = 0; i < facesNumber; i++){
        Mat temp;
        subtract(faces.col(i), avg, temp);
        temp = eigenVector * temp;
        temp.copyTo(trainFaceseign.col(i));
    }

    string facesPath = PATH + "faces.txt";
    ofstream writeFaceFile(facesPath.c_str(), ofstream::out | ofstream::trunc);

    for(int i = 0; i < PCA.col; i++){
        writeFaceFile << i+1 << ":";
        for (int j = 0; j < trainFaceseign.rows; j++) {
            writeFaceFile << trainFaceseign.col(i).at<float>(j);
            writeFaceFile << " ";
        }
        writeFaceFile << "\n";
    }

    writeFaceFile.close();


    string meanPath = PATH + "mean.txt";
    ofstream writeMeanFile(meanPath.c_str(), ofstream::out | ofstream::trunc);

    for (int i = 0; i < avg.rows; i++) {
        writeMeanFile << avg.at<float>(i);
        writeMeanFile << " ";
    }

    writeMeanFile.close();

    string eigenPath = PATH + "eigen.txt";
    ofstream writeEigenFile(eigenPath.c_str(), ofstream::out | ofstream::trunc);
    if (!writeEigenFile) {
        cout << "Fail to open file: " << eigenPath << endl;
    }

    for (int i = 0; i < eigenVector.rows; i++) {
        for (int j = 0; j < eigenVector.cols; j++) {
            writeEigenFile << eigenVector.row(i).at<float>(j);
            writeEigenFile << " ";
        }
        writeEigenFile << "\n";
    }

    writeEigenFile.close();

}


Mat TrainData::readFaces(int facesNumber, vector<string>& loadedFaceID){
    Mat faces = Mat::zeros(facesNumber, facesNumber, CV_32FC1);
    string facesPath = PATH + "faces.txt";
    ifstream readFaces(facesPath.c_str(), ifstream::in);

    string line, id;
    //    vector<string> loadedFaceID;
    loadedFaceID.clear();
    for (int i = 0; i < facesNumber; i++) {
        getline(readFaces, line);
        stringstream lines(line);
        getline(lines, id, ':');
        loadedFaceID.push_back(id);
        for (int j = 0; j < facesNumber; j++) {
            string data;
            getline(lines, data, ' ');
            faces.col(i).at<float>(j) = atof(data.c_str());
        }
    }

    readFaces.close();
    return faces;
}

Mat TrainData::readMean(){
    Mat mean = Mat::zeros(10000, 1, CV_32FC1);
    string meanPath = PATH + "mean.txt";
    ifstream readMean(meanPath.c_str(), ifstream::in);

    string line;
    for (int i = 0; i < 1; i++) {
        getline(readMean, line);
        stringstream lines(line);
        for (int j = 0; j < mean.rows; j++) {
            string data;
            getline(lines, data, ' ');
            mean.col(i).at<float>(j) = atof(data.c_str());
        }
    }

    readMean.close();
    return mean;
}


Mat TrainData::readEigen(int facesNumber){
    Mat eigen = Mat::zeros(facesNumber, 10000, CV_32FC1);
    string eigenPath = PATH + "eigen.txt";
    ifstream readEigen(eigenPath.c_str(), ifstream::in);

    string line;
    for (int i = 0; i < facesNumber; i++) {
        getline(readEigen, line);
        stringstream lines(line);
        for (int j = 0; j < eigen.cols; j++) {
            string data;
            getline(lines, data, ' ');
            eigen.at<float>(i,j) = atof(data.c_str());
        }
    }

    readEigen.close();

    return eigen;
}















