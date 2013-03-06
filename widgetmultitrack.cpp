#include "widgetmultitrack.h"
#include "ui_widgetmultitrack.h"
#include "widgettrack.h"
#include <math.h>
#include <QDebug>

WidgetMultiTrack::WidgetMultiTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMultiTrack),
    zoom(5000)
{
    ui->setupUi(this);
}

WidgetMultiTrack::~WidgetMultiTrack()
{
    delete ui;
}

WidgetTrack * WidgetMultiTrack::AddTrack()
{
    WidgetTrack * track = new WidgetTrack;
    tracks.push_back(track);
    ui->layoutTracks->addWidget(track);
    connect(track,SIGNAL(zoom(int,int,int)),this,SLOT(Zoomed(int,int,int)));
    connect(track,SIGNAL(mouseOver(int,QTime)),this,SIGNAL(MouseMoved(int,QTime)));
    connect(track,SIGNAL(signal_Scroll(int)),this,SLOT(Scrolled(int)));


    return track;
}


void WidgetMultiTrack::Zoomed(int delta,int pixel, int sampleIdx)
{
    //vypočitat počet pixelu na ktery interpolovat
    //vymyslet rozumnej krok
    //nechat interpolovat všechny tracky na stejnej počet
    //pixelu a stejnej počet vzorku na pixel

    if (delta > 0)
    {
        zoom /= 2;
    }
    else
        zoom *= 2;

    ui->label->setNum(zoom);
    //nechá interpolovat a překreslit všechny grafy
    //stejně podle toho kterej měl nejvic vzorku
    int oj;
    int size;
    int count;
    foreach(WidgetTrack * track, tracks)
    {
        track->Zoom(sampleIdx, pixel, zoom);
        track->repaint();
        oj = track->FirstPixel();
        size = track->rect().width();
        //předělat
        count = track->samples.count() / track->samplesPerPixel;
    }

    //předělat scrollbar
    ui->horizontalScrollBar->setMaximum(count - size*2/3);
    ui->horizontalScrollBar->setPageStep(size / 2);
    ui->horizontalScrollBar->setValue(oj);
}

void WidgetMultiTrack::on_horizontalScrollBar_actionTriggered(int)
{
    int position = ui->horizontalScrollBar->value();
    foreach(WidgetTrack * track, tracks)
    {
        int c = position * track->samplesPerPixel;
        track->Scroll(c);
        track->repaint();
    }
}

void WidgetMultiTrack::Scrolled(int delta)
{
    int val = ui->horizontalScrollBar->value();
    int step = ui->horizontalScrollBar->pageStep();
    if (delta > 0)
        ui->horizontalScrollBar->setValue(val + step);
    else
        ui->horizontalScrollBar->setValue(val - step);

    on_horizontalScrollBar_actionTriggered(0);
}
