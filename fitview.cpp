/* ToDo:
   - beim scrollen undefinierte Werte überspringen
*/

#include "fitview.h"
#include "ui_fitview.h"

#include "fit.h"
#include "chartscene.h"

#include <QStringList>
#include <QList>
#include <QString>
#include <QDebug>

fitView::fitView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fitView)
{
    ui->setupUi(this);

    ui->statusBar->showMessage(QString::fromUtf8("© Thorsten Gensler"));

    QStringList slToleranceZoneHole;
    slToleranceZoneHole << ""
                        << "A"
                        << "B"
                        << "C"
                        << "CD"
                        << "D"
                        << "E"
                        << "EF"
                        << "F"
                        << "FG"
                        << "G"
                        << "H"
                        << "JS"
                        << "J"
                        << "K"
                        << "M"
                        << "N"
                        << "P"
                        << "R"
                        << "S"
                        << "T"
                        << "U"
                        << "V"
                        << "X"
                        << "Y"
                        << "Z"
                        << "ZA"
                        << "ZB"
                        << "ZC";

    ui->cbToleranceZoneHole->addItems(slToleranceZoneHole);

    QStringList slToleranceZoneShaft;
    slToleranceZoneShaft << ""
                         << "a"
                         << "b"
                         << "c"
                         << "cd"
                         << "d"
                         << "e"
                         << "ef"
                         << "f"
                         << "fg"
                         << "g"
                         << "h"
                         << "js"
                         << "i"
                         << "k"
                         << "m"
                         << "n"
                         << "p"
                         << "r"
                         << "s"
                         << "t"
                         << "u"
                         << "v"
                         << "x"
                         << "y"
                         << "z"
                         << "za"
                         << "zb"
                         << "zc";

    ui->cbToleranceZoneShaft->addItems(slToleranceZoneShaft);

    QStringList slToleranceGrade;
    slToleranceGrade << "";
    for (int i = 1; i <= 18; i++) {
        slToleranceGrade << QString::number(i);
    }
    ui->cbToleranceGradeHole->addItems(slToleranceGrade);
    ui->cbToleranceGradeShaft->addItems(slToleranceGrade);

    QStringList verticalHeader;
    verticalHeader << QString(" Passungsart ")
                   << QString("")
                   << QString("")
                   << QString(" oberes Abmass Bohrung <b>[ES]</b> ")
                   << QString(" unteres Abmass Bohrung <b>[EI]</b> ")
                   << QString(" Hoechstmass Bohrung <b>[G<sub>oB</sub>]</b> ")
                   << QString(" Mindestmass Bohrung <b>[G<sub>uB</sub>]</b> ")
                   << QString(" oberes Abmass Welle <b>[es]</b> ")
                   << QString(" unteres Abmass Welle <b>[ei]</b> ")
                   << QString(" Hoechstmass Welle <b>[G<sub>oW</sub>]</b> ")
                   << QString(" Mindestmass Welle <b>[G<sub>uW</sub>]</b> ");
    //ui->tableWidget->setVerticalHeaderLabels(verticalHeader);
    verticalHeaderLabels = new QList<QLabel *>;
    for (int i = 0; i < verticalHeader.size(); ++i) {
        QLabel *headerLabel = new QLabel;
        headerLabel->setText(verticalHeader[i]);
        headerLabel->setAlignment(Qt::AlignRight);
        verticalHeaderLabels->append(headerLabel);
        ui->tableWidget->setCellWidget(i, 0, verticalHeaderLabels->last());
    }
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(TRUE);
    ui->tableWidget->setMinimumHeight(verticalHeader.size()
                                      * ui->tableWidget->verticalHeader()->defaultSectionSize()
                                      + 4);

    f = new Fit();

    connections();

    ui->cbN->click();
    ui->uiBasicHole->click();

    QRect *rectangle = new QRect;
    rectangle->setWidth(this->minimumWidth());
    rectangle->setHeight(this->minimumHeight());
    this->setGeometry(*rectangle);

    chart = new ChartScene(f);
    ui->graphicsView->setScene(chart);

    //ui->graphicsView->setMinimumWidth(200);
    //ui->graphicsView->setAlignment(Qt::AlignTop);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

}

fitView::~fitView()
{
    delete ui;
}

void fitView::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    // scale the scene to fit the view
    // not working atm
    case QEvent::Resize:
        ui->graphicsView->fitInView(0, 0, chart->width(), chart->height());
        break;
    default:
        break;
    }
}

void fitView::connections() {

    //NHole
    connect(ui->dsbNHole, SIGNAL(valueChanged(double)),
            f, SLOT(setNHole(double)));
    //NShaft
    connect(ui->dsbNShaft, SIGNAL(valueChanged(double)),
            f, SLOT(setNShaft(double)));

    //tolZoneHole
    connect(ui->cbToleranceZoneHole, SIGNAL(currentIndexChanged(QString)),
            f, SLOT(setTolZoneHole(QString)));

    //tolZoneShaft
    connect(ui->cbToleranceZoneShaft, SIGNAL(currentIndexChanged(QString)),
            f, SLOT(setTolZoneShaft(QString)));

    // cbN
    connect(ui->cbN, SIGNAL(clicked()),
            this, SLOT(cbNvalueChange()));

    //NHole = NShaft
    connect(ui->cbN, SIGNAL(clicked(bool)),
            ui->dsbNHole, SLOT(setHidden(bool)));

    //NHole = NShaft
    connect(ui->cbN, SIGNAL(clicked(bool)),
            ui->dsbNShaft, SLOT(setHidden(bool)));

    //tolGradeHole
    connect(ui->cbToleranceGradeHole, SIGNAL(currentIndexChanged(QString)),
            f, SLOT(setTolGradeHole(QString)));

    //tolGradeShaft
    connect(ui->cbToleranceGradeShaft, SIGNAL(currentIndexChanged(QString)),
            f, SLOT(setTolGradeShaft(QString)));

    //holeChanged
    connect(f, SIGNAL(holeChanged()),
            this, SLOT(setHoleChanged()));

    //shaftChanged
    connect(f, SIGNAL(shaftChanged()),
            this, SLOT(setShaftChanged()));

    //uiBasicHole
    connect(ui->uiBasicHole, SIGNAL(clicked(bool)),
            this, SLOT(setBasicHole(bool)));

    //uiBasicShaft
    connect(ui->uiBasicShaft, SIGNAL(clicked(bool)),
            this, SLOT(setBasicShaft(bool)));
}

void fitView::setHoleChanged() {
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem(f->getES()));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem(f->getEI()));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem(f->getGoB()));
    ui->tableWidget->setItem(6, 1, new QTableWidgetItem(f->getGuB()));
    setFit();
}

void fitView::setShaftChanged() {
    ui->tableWidget->setItem(7, 1, new QTableWidgetItem(f->getes()));
    ui->tableWidget->setItem(8, 1, new QTableWidgetItem(f->getei()));
    ui->tableWidget->setItem(9, 1, new QTableWidgetItem(f->getGoW()));
    ui->tableWidget->setItem(10, 1, new QTableWidgetItem(f->getGuW()));
    setFit();
}

void fitView::setFit() {
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(f->getFitKind()));
    verticalHeaderLabels->value(1)->setText(f->getFitDescriptionMax());
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(f->getFitMax()));
    verticalHeaderLabels->value(2)->setText(f->getFitDescriptionMin());
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem(f->getFitMin()));
}

void fitView::setBasicHole(bool b) {
    if (b) {
        ui->cbToleranceZoneHole->setCurrentIndex(11);
        ui->cbToleranceZoneHole->setDisabled(TRUE);
        ui->uiBasicShaft->setChecked(FALSE);
        ui->cbToleranceZoneShaft->setDisabled(FALSE);
    } else {
        ui->cbToleranceZoneHole->setDisabled(FALSE);
    }
}

void fitView::setBasicShaft(bool b) {
    if (b) {
        ui->cbToleranceZoneShaft->setCurrentIndex(11);
        ui->cbToleranceZoneShaft->setDisabled(TRUE);
        ui->uiBasicHole->setChecked(FALSE);
        ui->cbToleranceZoneHole->setDisabled(FALSE);
    } else {
        ui->cbToleranceZoneShaft->setDisabled(FALSE);
    }
}

void fitView::cbNvalueChange() {
    ui->dsbNHole->setValue(ui->dsbN->value());
    ui->dsbNShaft->setValue(ui->dsbN->value());
}
