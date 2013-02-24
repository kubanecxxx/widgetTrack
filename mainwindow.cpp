#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgettrack.h"
#include "widgetmultitrack.h"
#include <sndfile.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WidgetMultiTrack * multi = new WidgetMultiTrack;
    ui->verticalLayout->addWidget(multi);
    WidgetTrack * track = multi->AddTrack();
   // multi->AddTrack();

    SF_INFO info;
    memset(&info,0,sizeof(info));
    SNDFILE * file = sf_open("/home/kubanec/my\ music/numb.wav",SFM_READ,&info);

    QVector<float> data(info.frames);
    sf_read_float(file,data.data(),info.frames);

    track->PutData(data);
}

MainWindow::~MainWindow()
{
    delete ui;

}
