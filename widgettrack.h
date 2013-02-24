#ifndef WIDGETTRACK_H
#define WIDGETTRACK_H

#include <QWidget>

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
    
signals:
    void zoom(int delta,int pos);

private:
    Ui::WidgetTrack *ui;
    QVector<float> samples;
    QVector<float> interpolation;
    QVector<int> fitted;
    int center;
    int start;
    int stop;
    int Pixels;
    int samplesPerPixel;

    void Interpolate(int start, int stop, int pixels);
    void Interpolate(int pixels);
    void VerticalFit(int height);
    void Scroll(int center);
    void Zoom(int sample, int pos, int samplePerPixel);
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
