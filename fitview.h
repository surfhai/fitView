#ifndef FITVIEW_H
#define FITVIEW_H

#include "fit.h"
#include "chartscene.h"

#include <QMainWindow>
#include <QLabel>

namespace Ui {
    class fitView;
}

class fitView : public QMainWindow {
    Q_OBJECT
public:
    fitView(QWidget *parent = 0);
    ~fitView();

protected:
    void changeEvent(QEvent *e);

public slots:
    void setHoleChanged();
    void setShaftChanged();
    void setBasicHole(bool);
    void setBasicShaft(bool);
    void cbNvalueChange();

private:
    Ui::fitView *ui;
    Fit *f;
    ChartScene *chart;
    QList<QLabel *> *verticalHeaderLabels;
    void connections();
    void setFit();
};

#endif // FITVIEW_H
