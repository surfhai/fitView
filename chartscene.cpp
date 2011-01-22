#include "chartscene.h"
#include <QDebug>
#include <QGraphicsTextItem>
#include "fit.h"

ChartScene::ChartScene(Fit *f, QObject *parent) :
    QGraphicsScene(parent)
{
    fit = f;
    cBlack.setNamedColor("black");
    cBlue.setNamedColor("blue");
    cRed.setNamedColor("red");
    cWhite.setNamedColor("white");

    blackBrush.setColor(cBlack);
    blackBrush.setStyle(Qt::SolidPattern);
    blueBrush.setColor(cBlue);
    blueBrush.setStyle(Qt::SolidPattern);
    redBrush.setColor(cRed);
    redBrush.setStyle(Qt::SolidPattern);
    whiteBrush.setColor(cWhite);
    whiteBrush.setStyle(Qt::SolidPattern);

    blackPen.setColor(cBlack);
    bluePen.setColor(cBlue);
    redPen.setColor(cRed);
    whitePen.setColor(cWhite);

    font.setFamily("Sans Serif");
    font.setPixelSize(14);
    font.setBold(TRUE);

//    int test = this->width();
//    qDebug() << test;
    print();

    connect(fit, SIGNAL(holeChanged()),
            this, SLOT(chartHole()));
    connect(fit, SIGNAL(holeChanged()),
            this, SLOT(chartShaft()));
    connect(fit, SIGNAL(shaftChanged()),
            this, SLOT(chartHole()));
    connect(fit, SIGNAL(shaftChanged()),
            this, SLOT(chartShaft()));

}

void ChartScene::print() {
    image_height = 360;
    image_width = 300;
    startX = 20;

    QLine zeroLine(startX, image_height / 2, image_width - 50, image_height / 2);
    this->addLine(zeroLine, blackPen);

    QLine ordinateLine(startX, 0, startX, image_height);
    zeroLineItem = this->addLine(ordinateLine, blackPen);

    QPolygon ordinateArrowPositiv;
    ordinateArrowPositiv << QPoint(startX, 0)
                         << QPoint(startX - 3, 16)
                         << QPoint(startX + 3, 16);
    this->addPolygon(ordinateArrowPositiv, blackPen, blackBrush);

    QPolygon ordinateArrowNegativ;
    ordinateArrowNegativ << QPoint(startX, image_height)
                         << QPoint(startX - 3, image_height - 16)
                         << QPoint(startX + 3, image_height - 16);
    this->addPolygon(ordinateArrowNegativ, blackPen, blackBrush);

    QLine basicLine(startX + 30, image_height / 2, startX + 30, image_height / 5 * 4);
    this->addLine(basicLine);

    QPolygon basicArrow;
    basicArrow << QPoint(startX + 30, image_height / 2)
               << QPoint(startX + 27, image_height / 2 + 16)
               << QPoint(startX + 33, image_height / 2 + 16);
    this->addPolygon(basicArrow, blackPen, blackBrush);

    QGraphicsTextItem *sBasicSize = new QGraphicsTextItem(
            QString::fromUtf8("Nennmaß"));
    sBasicSize->setFont(font);
    sBasicSize->rotate(-90);
    sBasicSize->setPos(startX + 30 - font.pixelSize() - 6,
                       image_height / 2 + 28 + sBasicSize->boundingRect().width());
    this->addItem(sBasicSize);

    QGraphicsTextItem *sZero = new QGraphicsTextItem("0");
    sZero->setFont(font);
    sZero->setPos(0, (image_height / 2) - sZero->boundingRect().height() / 2);
    this->addItem(sZero);

    QGraphicsTextItem *sPositiv = new QGraphicsTextItem("+");
    sPositiv->setFont(font);
    sPositiv->setPos(0, image_height / 2 - 50 - sPositiv->boundingRect().height() / 2);
    this->addItem(sPositiv);

    QGraphicsTextItem *sNegativ = new QGraphicsTextItem("-");
    sNegativ->setFont(font);
    sNegativ->setPos(0, image_height / 2 + 50 - sNegativ->boundingRect().height() / 2);
    this->addItem(sNegativ);

    //QRect *quadrat

    QGraphicsTextItem *sHole = new QGraphicsTextItem("Welle");
    sHole->setFont(font);
    sHole->setPos(startX + 30,
                  image_height + font.pixelSize());
    this->addItem(sHole);

    QGraphicsTextItem *sShaft = new QGraphicsTextItem("Bohrung");
    sShaft->setFont(font);
    sShaft->setPos(startX + 150,
                   image_height + font.pixelSize());
    this->addItem(sShaft);

    QRect *holeRect = new QRect(startX + 130,
                                image_height + font.pixelSize() * 1.4,
                                font.pixelSize(),
                                font.pixelSize());
    this->addRect(*holeRect, bluePen, blueBrush);
    QRect *shaftRect = new QRect(startX + 10,
                          image_height + font.pixelSize() * 1.4,
                          font.pixelSize(),
                          font.pixelSize());
    this->addRect(*shaftRect, redPen, redBrush);

    QRect *tempRect = new QRect;

    holeItem = this->addRect(*tempRect, bluePen, blueBrush);
    shaftItem = this->addRect(*tempRect, redPen, redBrush);

    zeroLineItem->setZValue(100);
}

void ChartScene::chartHole() {
    if (!fit->isHoleDefined()) {
        holeItem->hide();
        return;
    } else {
        holeItem->show();
        double rate = getRate();
        holeItem->setRect(image_width / 5 * 2 + 20,
                          image_height / 2 - fit->getDoubleES() * rate,
                          image_width / 3,
                          (fit->getDoubleES() - fit->getDoubleEI()) * rate);
        holeItem->setZValue(10);
    }
}

void ChartScene::chartShaft() {
    if (!fit->isShaftDefined()) {
        shaftItem->hide();
        return;
    } else {
        shaftItem->show();
        double rate = getRate();
        if (rate == 0.0)
            return;
        shaftItem->setRect(image_width / 5 + 20,
                          image_height / 2 - fit->getDoublees() * rate,
                          image_width / 3,
                          (fit->getDoublees() - fit->getDoubleei()) * rate);
        shaftItem->setZValue(20);
    }
}

double ChartScene::getRate() {
    QList<double> dim;
    if (fit->isHoleDefined()) {
        dim << fit->getDoubleES()
            << fit->getDoubleEI();
    }
    if (fit->isShaftDefined()) {
        dim << fit->getDoublees()
            << fit->getDoubleei();
    }
    if(dim.isEmpty())
        return 0;
    qSort(dim.begin(), dim.end());
    return (image_height / 2.0 / qMax(qAbs(dim.first()),qAbs(dim.last())));
}
