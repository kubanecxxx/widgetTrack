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
    connect(multi,SIGNAL(MouseMoved(int,QTime)),this,SLOT(MouseHovered(int,QTime)));

    SF_INFO info;
    memset(&info,0,sizeof(info));
    SNDFILE * file = sf_open("/home/kubanec/my\ music/numb.wav",SFM_READ,&info);

    QVector<float> data(info.frames);
    sf_read_float(file,data.data(),info.frames);

    QVector<float> hoje(info.frames / 2);
    for (int i = 0 ; i < hoje.count(); i++)
    {
        hoje[i] = data[i*2];
    }

    track->PutData(hoje);
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::MouseHovered(int pos, QTime time)
{
    statusBar()->showMessage(QString("%1 / %2").arg(pos).arg(time.toString()));
}
