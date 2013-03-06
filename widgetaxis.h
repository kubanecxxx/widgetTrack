#ifndef WIDGETAXIS_H
#define WIDGETAXIS_H

#include <QWidget>

namespace Ui {
class WidgetAxis;
}

class WidgetAxis : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetAxis(QWidget *parent = 0);
    ~WidgetAxis();
    
private:
    Ui::WidgetAxis *ui;
    void paintEvent(QPaintEvent *);

    int FirstSample;
    float SamplesPerPixel;

    friend class WidgetMultiTrack;
};

#endif // WIDGETAXIS_H
