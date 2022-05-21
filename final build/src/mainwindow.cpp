#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QObject::connect(ui->Ma1, &QPushButton::clicked, this, &MainWindow::MAX4);
    connect(ui->actionOpenFolder, SIGNAL(triggered()), this, SLOT(openFolder()));


    QObject::connect(ui->axialSlider, &QSlider::valueChanged,
            this, &MainWindow::axialSliderCtrl);
    QObject::connect(ui->coronalSlider, &QSlider::valueChanged,
            this, &MainWindow::coronalSliderCtrl);
    QObject::connect(ui->sagittalSlider, &QSlider::valueChanged,
            this, &MainWindow::sagittalSliderCtrl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::axialSliderCtrl(){
    int id = ui->axialSlider->value();
    MainWindow::axialViewCtrl(id);
}
void MainWindow::coronalSliderCtrl(){
    int id = ui->coronalSlider->value();
    MainWindow::coronalViewCtrl(id);
}
void MainWindow::sagittalSliderCtrl(){
    int id = ui->sagittalSlider->value();
    MainWindow::sagittalViewCtrl(id);
}

void MainWindow::axialViewCtrl(int id){
    sceneManager.sceneAxial = new Scenez(this);
    QPixmap axialPixmap= ASM::cvMatToQPixmap(*this->imageManager.axialImages[id]);
    sceneManager.sceneAxial->addPixmap(axialPixmap);
    QRectF rc(QPoint(0, 0), axialPixmap.size());
    ui->axialView->fitInView(rc, Qt::KeepAspectRatio);
    ui->axialView->setScene(sceneManager.sceneAxial);
}
void MainWindow::coronalViewCtrl(int id){
    sceneManager.sceneCoronal = new Scenez(this);
    QPixmap coronalPixmap= ASM::cvMatToQPixmap(*this->imageManager.coronalImages[id]);
    sceneManager.sceneCoronal->addPixmap(coronalPixmap);
    QRectF rc(QPoint(0, 0), coronalPixmap.size());
    ui->coronalView->fitInView(rc, Qt::KeepAspectRatio);
    ui->coronalView->setScene(sceneManager.sceneCoronal);
}
void MainWindow::sagittalViewCtrl(int id){
    sceneManager.sceneSagittal = new Scenez(this);
    QPixmap sagittalPixmap= ASM::cvMatToQPixmap(*this->imageManager.sagittalImages[id]);
    sceneManager.sceneSagittal->addPixmap(sagittalPixmap);
    QRectF rc(QPoint(0, 0), sagittalPixmap.size());
    ui->sagittalView->fitInView(rc, Qt::KeepAspectRatio);
    ui->sagittalView->setScene(sceneManager.sceneSagittal);
}

void MainWindow::setOrthogonalViews(){
    MainWindow::axialViewCtrl(0);
    MainWindow::coronalViewCtrl(0);
    MainWindow::coronalViewCtrl(0);
}
void MainWindow::setSliders(){
    ui->axialSlider->setRange(0,this->imageManager.axialImages.size()-1);
    ui->axialSlider->setValue(0);
    //std::cout<<this->imageManager.axialImages.size()<<std::endl;

    ui->coronalSlider->setRange(0,this->imageManager.coronalImages.size()-1);
    ui->coronalSlider->setValue(0);
    //std::cout<<this->imageManager.coronalImages.size()<<std::endl;

    ui->sagittalSlider->setRange(0,this->imageManager.sagittalImages.size()-1);
    ui->sagittalSlider->setValue(0);
    //std::cout<<this->imageManager.sagittalImages.size()<<std::endl;
}


void MainWindow::openFolder(){
    folderPath= QFileDialog::getExistingDirectory(this,
            tr("Open Folder"),
            "C:\\",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir  Directory = folderPath;
    QStringList DICOMS = Directory.entryList(QStringList()<<"*.dcm", QDir::Files);
    foreach (QString Dicom, DICOMS){
        DICOM_Names.push_back(Dicom.toUtf8().constData());
    }
    SI::natural::sort(DICOM_Names);
    for(int i =0;i<DICOM_Names.size();i++){
         Full_Path_Names.push_back(folderPath.toStdString() + "//" + DICOM_Names[i]);
    }
    this->imageManager.header.loadInfo(folderPath.toStdString());
    this->imageManager.loadImages(Full_Path_Names);
    this->imageManager.grayTransform();
    this->imageManager.MPR();
    MainWindow::setSliders();
    MainWindow::setOrthogonalViews();
}