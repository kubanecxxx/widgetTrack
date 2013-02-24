#include "widgettrack.h"
#include "ui_widgettrack.h"
#include <math.h>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

WidgetTrack::WidgetTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTrack),
    pressed(false),
    center(0),
    start(0),
    stop(0)
{
    ui->setupUi(this);

    /*******************************************************************
     * zoom v ose x
     * posun v ose x
     * výběr v ose x
     * automaticky měřitko nedělat, uživatel si sám musi zoomnout
     * popisek osy x - čas/vzorky
     * nějaké title
     * pravy tlačitko context menu
     * tootlip kde sme s čislem vzorku
     ******************************************************************/

}

WidgetTrack::~WidgetTrack()
{
    delete ui;
}

void WidgetTrack::Replot()
{
    Interpolate(500);
    VerticalFit(rect().height());
}

void WidgetTrack::PutData(QVector<float> &data)
{
    samples = data;
    start = 0;
    stop = data.count() - 1;
    center = stop/2;
    Interpolate(500);
    repaint();
}

/** ****************************************************************
 * @brief
 * smrskne všechny vrozky do malyho pole delky pixels
 * v horizontálním směru
 ******************************************************************/
void WidgetTrack::Interpolate(int start, int stop, int pixels)
{
    interpolation.clear();
    interpolation.resize(pixels);
    Pixels = pixels;

    int samplesInPixel = (stop - start) / pixels;
    samplesPerPixel = samplesInPixel;

    for (int i = 0 ; i < pixels - 1; i++)
    {
        float result = 0;
        if (samplesInPixel * (i+1) > samples.count())
        {
            result = 0;
        }
        else
        {
            for (int j = i * samplesInPixel; j < samplesInPixel * (i+1);j++ )
                result += samples[j] * samples[j];
        }
        result /= samplesInPixel;
        interpolation[i] = sqrt(result);
    }
}

void WidgetTrack::Interpolate(int pixels)
{
    Interpolate(0,samples.count(), pixels);
}

void WidgetTrack::VerticalFit(int height)
{
    fitted.clear();
    fitted.resize(interpolation.count());

    int i = 0;
    foreach(float sample, interpolation)
    {
        fitted[i] = sample * height;
        i++;
    }
}

void WidgetTrack::paintEvent(QPaintEvent *)
{
    if (interpolation.empty())
        return;

    QPainter p;

    int height = rect().height();
    VerticalFit(height);
    p.begin(this);

    p.setPen(Qt::red);
    p.fillRect(point.x(),5,point2.x() - point.x(),rect().height()- 10,Qt::gray);

    p.setPen(Qt::blue);

    int b = 0;
    for (int i = start / samplesPerPixel ; i < stop / samplesPerPixel -1; i++, b++)
        if (i < fitted.count() && b < rect().width())
            p.drawLine(b,height/2 + fitted[i],b,height/2-fitted[i]);

    p.setPen(Qt::black);
    p.drawLine(0,height/2,rect().width(),height/2);
    p.drawRect(0,0,rect().width()-1,rect().height()-1);
}

void WidgetTrack::mouseMoveEvent(QMouseEvent * evt)
{
    if (pressed)
    {
        point2 = evt->pos();
        repaint();

    }
}

void WidgetTrack::mousePressEvent(QMouseEvent * evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        pressed = true;
        point = evt->pos();
    }
    QWidget::mousePressEvent(evt);
}

void WidgetTrack::mouseReleaseEvent(QMouseEvent * evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        pressed = false;
        point2 = evt->pos();
        repaint();
    }
}

void WidgetTrack::wheelEvent(QWheelEvent * wheel)
{
    int delta = wheel->delta();
  //  emit zoom(delta, wheel->x() * samplesPerPixel + start);

    int co;
    if (delta > 0)
        co = Pixels * 2;
    else
        co = Pixels / 2;
    Zoom(wheel->x() * samplesPerPixel + start, wheel->x(),co);
}

/** ****************************************************************
 * @brief nechá zoomnout graf
 * @param sample vzorek na kterej se má zoomovat
 * @param pos pozice vzorku v pixelech
 * @param samplePerPixel míra zoomu, kolik vzorků na pixel
 ******************************************************************/
void WidgetTrack::Zoom(int sample, int pos, int pixels)
{
    Interpolate(pixels);


}

//cílem je vypočitat start a stop
void WidgetTrack::Scroll(int center) //číslo vzorku
{
    int samples = rect().width() * samplesPerPixel;

    start = center;
    if (start < 0)
        start = 0;

    stop = start + samples;
    if (stop > this->samples.count())
        stop = this->samples.count();
}
