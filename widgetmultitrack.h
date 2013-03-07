#ifndef WIDGETMULTITRACK_H
#define WIDGETMULTITRACK_H

#include <QWidget>
#include <QTime>

namespace Ui {
class WidgetMultiTrack;
}

class QLabel;
class WidgetTrack;
class WidgetMultiTrack : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetMultiTrack(QWidget *parent = 0);
    ~WidgetMultiTrack();
    WidgetTrack * AddTrack();
    inline int GetTrackCount() const {return tracks.count();}
    inline WidgetTrack * GetTrack(int i) const {return tracks.at(i);}
    void RemoveTrack(WidgetTrack * track);

signals:
    void MouseMoved(int pos, QTime time);

private:
    Ui::WidgetMultiTrack *ui;
    QList<WidgetTrack *> tracks;

    //počet pixelů do kterejch se to má cely narvat
    float zoom;

private slots:
    void Zoomed(int delta, int pixel, int sample);
    void Scrolled(int delta);

    void on_horizontalScrollBar_actionTriggered(int action);
};

#endif // WIDGETMULTITRACK_H
