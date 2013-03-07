#include "widgettrack.h"
#include "ui_widgettrack.h"
#include <math.h>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QTime>

WidgetTrack::WidgetTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTrack),
    pressed(false),
    center(0),
    start(0),
    stop(0),
    SelectionStart(0),
    SelectionStop(0),
    samplesPerPixel(10000)
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

    setMouseTracking(true);
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

void WidgetTrack::AddData(QVector<float> &data)
{
    samples += data;
    Scroll(samples.count() - rect().width() * samplesPerPixel);
    repaint();
}

/** ****************************************************************
 * @brief
 * smrskne všechny vrozky do malyho pole delky pixels
 * v horizontálním směru
 ******************************************************************/
void WidgetTrack::Interpolate(int start, int stop, float pixels)
{
    //ruzny režimy interpolace
    //ten co je teď
    //pro pár vzorku na pixel
    // a žádná pro 1 a miň vzorku na pixel
    //taky počitat s nulama na začátku
    // a nepočitat to zbytečně
    //nebo to cely neřešit a přidávat nulovy vzorky
    //a počitat interpolaci furt aji z nulovéch vzorku

    samplesPerPixel = pixels;

    //určit typ interpolace
    if (pixels <= 1)
        InterpolationType = NONE;
    else if (pixels <= 10)
        InterpolationType = LITTLE;
    else
        InterpolationType = FULL;

    switch (InterpolationType)
    {
    case NONE:

        break;

    case LITTLE:

        break;

    case FULL:
        interpolation.clear();
        Pixels =  (stop - start) / samplesPerPixel;
        interpolation.resize(stop / samplesPerPixel);
        interpolation.fill(0);
        int samplesInPixel = samplesPerPixel;
        for (int i = start/samplesPerPixel ; i < interpolation.count(); i++)
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
        break;
    }
}

void WidgetTrack::Interpolate(float pixels)
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
    QPainter p;
    p.begin(this);
    p.setPen(Qt::blue);
    int height = rect().height();

    /*
     * vykreslit výběr
     */
    int startPixel = (SelectionStart - start) / samplesPerPixel ;
    int stopPixel =(SelectionStop - start) / samplesPerPixel ;
    p.fillRect(startPixel,5,stopPixel - startPixel , height - 10,Qt::gray);


    int b = 0;
    float ch = 0;
    float alte = 0;
    float alteS  = 0;
    switch (InterpolationType)
    {
    case NONE:
        for (int i = start ; i < stop ; i++)
        {
            float sample = samples[i] * height / 2+ height / 2; //fit
            ch += 1/samplesPerPixel;
            p.setPen(Qt::red);
            p.drawEllipse(ch,sample,2,2);

            p.setPen(Qt::blue);
            if (ch > 1/samplesPerPixel )
                p.drawLine(alte,alteS,ch,sample);
            alte = ch;
            alteS = sample;
        }
        break;

    case LITTLE:
    case FULL:
        if (interpolation.empty())
            return;

        VerticalFit(height);
        for (int i = start / samplesPerPixel ; i < stop / samplesPerPixel -1; i++, b++)
            if (i < fitted.count() && b < rect().width())
                p.drawLine(b,height/2 + fitted[i],b,height/2-fitted[i]);
        break;
    }
    p.setPen(Qt::black);
    p.drawLine(0,height/2,rect().width(),height/2);
    p.drawRect(0,0,rect().width()-1,rect().height()-1);
}

void WidgetTrack::mouseMoveEvent(QMouseEvent * evt)
{
    int sample = evt->pos().x() * samplesPerPixel + start;
    emit mouseOver( sample, QTime::currentTime());

    //setToolTip(QString("Vzorek %1").arg(sample));
    if (pressed)
    {
        SelectionStop = evt->pos().x() * samplesPerPixel + start ;
        repaint();
    }
}

void WidgetTrack::mousePressEvent(QMouseEvent * evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        pressed = true;
        SelectionStart = evt->pos().x() * samplesPerPixel + start ;
    }
    QWidget::mousePressEvent(evt);
}

void WidgetTrack::mouseReleaseEvent(QMouseEvent * evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        pressed = false;
        SelectionStop = evt->pos().x() * samplesPerPixel + start ;
        repaint();
    }
}

void WidgetTrack::wheelEvent(QWheelEvent * wheel)
{
    int delta = wheel->delta();

    if (wheel->modifiers().testFlag(Qt::ControlModifier))
        emit zoom(delta, wheel->x() ,wheel->x() * samplesPerPixel + start);
    else
        emit signal_Scroll(delta);
}

/** ****************************************************************
 * @brief nechá zoomnout graf
 * @param sample vzorek na kterej se má zoomovat
 * @param pos pozice vzorku v pixelech
 * @param samplePerPixel míra zoomu, kolik vzorků na pixel
 ******************************************************************/
void WidgetTrack::Zoom(int sample, int pos, float pixels)
{
    samplesPerPixel = pixels;
    int center = (int) (sample - (pos * samplesPerPixel));
    Scroll(center);
}

//cílem je vypočitat start a stop
void WidgetTrack::Scroll(int center) //číslo pixelu
{
    int samples = rect().width() * samplesPerPixel;

    start = center;
    if (start < 0)
        start = 0;

    stop = start + samples;
    if (stop > this->samples.count())
        stop = this->samples.count();

    Interpolate(start,stop,samplesPerPixel);
}

int WidgetTrack::FirstPixel()
{
    return (start / samplesPerPixel);
}

QVector<float> WidgetTrack::GetSelectedSamples() const
{
    QVector<float> samp;
    samp = samples.mid(SelectionStart, SelectionStop - SelectionStart);
    return samp;
}
