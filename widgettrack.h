#ifndef WIDGETTRACK_H
#define WIDGETTRACK_H

#include <QWidget>
#include <QTime>

namespace Ui {
class WidgetTrack;
}


class WidgetTrack : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetTrack(QWidget *parent = 0);
    ~WidgetTrack();
    void PutData(QVector<float> & data);
    void AddData(QVector<float> & data);
    int FirstPixel();
    
signals:
    void zoom(int delta,int pos, int sample);
    void mouseOver(int sample, QTime time);
    void signal_Scroll(int delta);

private:
    typedef enum {NONE, FULL, LITTLE} enumInterpolation_t;

    Ui::WidgetTrack *ui;
    QVector<float> samples;
    QVector<float> interpolation;
    QVector<int> fitted;
    int center;
    int start;
    int stop;
    int Pixels;     /// celá track v pixelech
    float samplesPerPixel;
    int SelectionStart;
    int SelectionStop;

    void Interpolate(int start, int stop, float samplesPerPixel);
    void Interpolate(float pixels);
    void VerticalFit(int height);
    void Scroll(int FirstSample);
    void Zoom(int sample, int pos, float samplesPerPixel);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

    bool pressed;
    enumInterpolation_t InterpolationType;

    void Draw();
    void Replot();

    friend class WidgetMultiTrack;
};

#endif // WIDGETTRACK_H
