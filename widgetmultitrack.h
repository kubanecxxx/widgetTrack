#ifndef WIDGETMULTITRACK_H
#define WIDGETMULTITRACK_H

#include <QWidget>

namespace Ui {
class WidgetMultiTrack;
}

class WidgetTrack;
class WidgetMultiTrack : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetMultiTrack(QWidget *parent = 0);
    ~WidgetMultiTrack();
    WidgetTrack * AddTrack();

private:
    Ui::WidgetMultiTrack *ui;
    QList<WidgetTrack *> tracks;

    //počet pixelů do kterejch se to má cely narvat
    int zoom;

private slots:
    void Zoomed(int delta, int x);
    void on_horizontalScrollBar_sliderMoved(int position);
};

#endif // WIDGETMULTITRACK_H
