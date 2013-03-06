#include "widgetaxis.h"
#include "ui_widgetaxis.h"
#include <QPainter>

WidgetAxis::WidgetAxis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetAxis)
{
    ui->setupUi(this);
}

WidgetAxis::~WidgetAxis()
{
    delete ui;
}

#define CARKY 50
#define POPISKY 3

void WidgetAxis::paintEvent(QPaintEvent *)
{
    QPainter p;
    p.begin(this);

   /*
    * spočitat rozumny měřitko z velikosti a počtu vzorku na pixel
    * třeba po deseti px udělat čárečku
    * a po 100px napsat čislo vzorčeku nebo čas
    *
    * spočitat si kolik tam bude vzorku a nějaky kulaty čislo hodit
    * pak vuči němu posunout zbytek
    *
    */

    //offset se musi šikovně spočitat
    int offset = 0;

    for (int i = 0 ; i < rect().width() / CARKY + 1; i++ )
    {
        int x = i * CARKY + offset;
        p.drawLine(x,0,x,5);

        if ((i % (POPISKY)) == 0)
        {
            p.drawLine(x,0,x,10);
        }
    }
}
