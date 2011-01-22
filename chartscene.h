#ifndef CHARTSCENE_H
#define CHARTSCENE_H

#include <QGraphicsScene>
#include "fit.h"

class ChartScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit ChartScene(Fit *f, QObject *parent = 0);

signals:

public slots:
    void chartHole();
    void chartShaft();

private:
    Fit *fit;
    void print();
    QColor cBlack;
    QColor cBlue;
    QColor cRed;
    QColor cWhite;

    QBrush blackBrush;
    QBrush blueBrush;
    QBrush redBrush;
    QBrush whiteBrush;

    QPen blackPen;
    QPen bluePen;
    QPen redPen;
    QPen whitePen;

    QFont font;

    int image_height;
    int image_width;
    int startX;

    QGraphicsRectItem *holeItem;
    QGraphicsRectItem *shaftItem;
    QGraphicsLineItem *zeroLineItem;

    double getRate();

};

#endif // CHARTSCENE_H
