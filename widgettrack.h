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

private:
    Ui::WidgetTrack *ui;
    QVector<float> samples;
    QVector<float> interpolation;
    QVector<int> fitted;
    int center;
    int start;
    int stop;
    int Pixels;     /// celá track v pixelech
    int samplesPerPixel;

    void Interpolate(int start, int stop, int samplesPerPixel);
    void Interpolate(int pixels);
    void VerticalFit(int height);
    void Scroll(int FirstSample);
    void Zoom(int sample, int pos, int samplesPerPixel);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

    bool pressed;
    QPoint point,point2;

    void Draw();
    void Replot();

    friend class WidgetMultiTrack;
};

#endif // WIDGETTRACK_H
