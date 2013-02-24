#include "widgetmultitrack.h"
#include "ui_widgetmultitrack.h"
#include "widgettrack.h"
#include <math.h>

WidgetMultiTrack::WidgetMultiTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMultiTrack),
    zoom(500)
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
    connect(track,SIGNAL(zoom(int,int)),this,SLOT(Zoomed(int,int)));

    return track;
}


void WidgetMultiTrack::Zoomed(int delta,int sample)
{
    //vypočitat počet pixelu na ktery interpolovat
    //vymyslet rozumnej krok
    //nechat interpolovat všechny tracky na stejnej počet
    //pixelu a stejnej počet vzorku na pixel

    if (delta > 0)
        zoom *= 2;
    else
        zoom /= 2;

    //vybere graf kterej má nejvic vzorku
    QVector<int> sizes;
    foreach(WidgetTrack * track, tracks)
        sizes.push_back(track->samples.count());

    qSort(sizes);
    int samples = sizes.at(sizes.count() - 1);

    //nechá interpolovat a překreslit všechny grafy
    //stejně podle toho kterej měl nejvic vzorku
    foreach(WidgetTrack * track, tracks)
    {
        track->Interpolate(0,samples,zoom);
        track->Scroll(sample);
        track->repaint();
    }
}


void WidgetMultiTrack::on_horizontalScrollBar_sliderMoved(int position)
{
    foreach(WidgetTrack * track, tracks)
    {
        int c = position * track->samplesPerPixel;
        track->Scroll(c);
        track->repaint();
    }
}
