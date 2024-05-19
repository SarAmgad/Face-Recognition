#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lables->addItem("Choose class");
    ui->lables->addItem("Gehad");
    ui->lables->addItem("Rawda");
    ui->lables->addItem("Hager");
    ui->lables->addItem("Sara");
    ui->lables->addItem("Rawan");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readImage(QLabel *img_lbl){
    image = imread(file_name.toStdString(), IMREAD_COLOR);
    displayImage(img_lbl, image);
}

void MainWindow::displayImage(QLabel *img_lbl, Mat img){
    cvtColor(img, imageRGB, COLOR_BGR2RGB);
    img_lbl->setPixmap(QPixmap::fromImage(QImage(imageRGB.data, imageRGB.cols, imageRGB.rows, imageRGB.step,
                                                 QImage::Format_RGB888)).scaled(w,h,Qt::KeepAspectRatio));
    img_lbl->repaint();
}

void MainWindow::on_Browse_clicked()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Images (*.png *.xpm *.jpg)"));
    if (!file_name.isEmpty()){
        readImage(ui->input);
    }
}


void MainWindow::on_recognize_clicked()
{
    if (!file_name.isEmpty()){
        FaceDetection face;
        Mat ROI;
        Mat processed;
        Mat img = face.detectFaces(file_name.toStdString(), ROI);
        displayImage(ui->input, img);

        TrainData read;
        vector<string> loadedFaceID;
        Mat faces = read.readFaces(30, loadedFaceID);
        Mat avg = read.readMean();
        Mat eigens = read.readEigen(30);

        Recognize recognize;
        ROI.convertTo(ROI, CV_32FC1);
        string FaceID = recognize.getID(ROI, avg, eigens, faces, loadedFaceID);
        string FaceName;
        int id = std::stoi(FaceID);


        if(id >= 1 && id <= 6){
            FaceName = "Gehad";
        }
        else if(id >= 7 && id <= 12){
            FaceName = "Rawda";
        }
        else if(id >= 13 && id <= 18){
            FaceName = "Hagar";
        }
        else if(id >= 19 && id <= 24){
            FaceName = "Sara";
        }
        else if(id >= 25 && id <= 30){
            FaceName = "Rawan";
        }
        else{
            FaceName = "None";
        }

        ui->faceName->setText(QString::fromStdString(FaceName));
    }
}

void MainWindow::on_train_clicked()
{
    TrainData train;
    train.WriteData();
}


void MainWindow::on_lables_activated(int index)
{
    lable = index;
}


void MainWindow::on_k_value_textChanged(const QString &arg1)
{
    k = ui->k_value->value();
}


void MainWindow::on_rocButton_clicked()
{
    cout << lable;
    Recognize roc;
    Mat img;
    if(lable == 1){
        img = roc.performance(1, 6, k);
    }
    else if(lable == 2){
        img = roc.performance(7, 12, k);
    }
    else if(lable == 3){
        img = roc.performance(13, 18, k);
    }
    else if(lable == 4){
        img = roc.performance(19, 24, k);
    }
    else if(lable == 5){
        img = roc.performance(25, 30, k);
    }
    else if(lable == 0){
        //
    }
    displayImage(ui->roc, img);
}

