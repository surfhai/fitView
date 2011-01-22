/* ToDo:
   -
*/

#include "fit.h"
#include <QFile>
#include <QVector>
#include <QString>
#include <QRegExp>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <iostream>

Fit::Fit(QObject *parent) :
    QObject(parent)
{
    // init variables
    NHole = 0;
    NShaft = 0;
    ES = 0;
    EI = 0;
    es = 0;
    ei = 0;
    tolZoneHole = "";
    tolZoneShaft = "";
    tolGradeHole = 0;
    tolGradeShaft = 0;
    fundTolHole = 0;
    fundTolShaft = 0;
    fundDevHole = "";
    fundDevShaft = "";
    holeNotDefined = 0;
    shaftNotDefined = 0;
    fitKind = "";
    maxP = 0;
    minP = 0;
    fitReady = 0;


    // fill fundamentalTolerance with data
    //QFile file("resources/FundamentalTolerance.csv");
    QFile fileTol(":/resources/FundamentalTolerance.csv");
    if (fileTol.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileTol);
        QString line = in.readLine();
        while (!in.atEnd()) {
            QString line = in.readLine();

            line.replace(QRegExp(","), ".");
            QStringList list1 = line.split(";");
            QVector<double> dvector;

            for (int i = 0; i < list1.size(); ++i)
                dvector.append(list1[i].toDouble());
            fundamentalTol.append(dvector);
        }

    }
    QFile fileDevHole(":/resources/FundamentalDeviationHole.csv");
    if (fileDevHole.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileDevHole);
        while (!in.atEnd()) {
            QString line = in.readLine();

            line.replace(QRegExp(","), ".");
            QStringList list1 = line.split(";");

            QVector<QString> svector;

            for (int i = 0; i < list1.size(); ++i)
                svector.append(list1[i]);
            fundamentalDevHole.append(svector);
        }
    }

    QFile fileDevShaft(":/resources/FundamentalDeviationShaft.csv");
    if (fileDevShaft.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileDevShaft);
        while (!in.atEnd()) {
            QString line = in.readLine();

            line.replace(QRegExp(","), ".");
            QStringList list1 = line.split(";");

            QVector<QString> svector;

            for (int i = 0; i < list1.size(); ++i)
                svector.append(list1[i]);
            fundamentalDevShaft.append(svector);
        }
    }
}

void Fit::setNHole(double value) {
    if (value != NHole) {
        NHole = value;
        delSetHole();
        emit valueChanged();
    }
}

void Fit::setNShaft(double value) {
    if (value != NShaft) {
        NShaft = value;
        delSetShaft();
        emit valueChanged();
    }
}

void Fit::setTolZoneHole(QString s) {
    if (s != tolZoneHole) {
        tolZoneHole = s;
        emit valueChanged();
    }
}

void Fit::setTolZoneShaft(QString s) {
    if (s != tolZoneShaft) {
        tolZoneShaft = s;
        emit valueChanged();
    }
}

void Fit::setTolGradeHole(QString s) {
    int value = s.toInt();
    if (value != tolGradeHole) {
        tolGradeHole = value;
        delSetHole();
        emit valueChanged();
    }
}

void Fit::setTolGradeShaft(QString s) {
    int value = s.toInt();
    if (value != tolGradeShaft) {
        tolGradeShaft = value;
        delSetShaft();
        emit valueChanged();
    }
}

void Fit::valueChanged() {
    if (holeReady()) {
        setHole();
        calcHole();
        debugOutput();
    } else {
        holeNotDefined = 0;
        delFit();
    }
    emit holeChanged();
    if (shaftReady()) {
        setShaft();
        calcShaft();
        debugOutput();
    } else {
        shaftNotDefined = 0;
        delFit();
    }
    emit shaftChanged();
}

bool Fit::isHoleDefined() {
//    qDebug() << "holeReady" << holeReady();
//    qDebug() << "holeNotDefined" << holeNotDefined;
//    qDebug() << "(!holeNotDefined && holeReady())" << (!holeNotDefined && holeReady());
    return (!holeNotDefined && holeReady());
}

bool Fit::isShaftDefined() {
    return (!shaftNotDefined && shaftReady());
}

QString Fit::getES() {
    QString str;
    if (holeNotDefined)
        str = "  undefined";
    else if (holeReady()) {
        str.setNum(ES);
        str.insert(0, "  ");
        str.append(QString::fromUtf8(" µm"));
    }
    else
        str = "";
    return str;
}

double Fit::getDoubleES() {
    return ES;
}

QString Fit::getEI() {
    QString str;
    if (holeNotDefined)
        str = "  undefined";
    else if (holeReady()) {
        str.setNum(EI);
        str.insert(0, "  ");
        str.append(QString::fromUtf8(" µm"));
    }
    else
        str = "";
    return str;
}

double Fit::getDoubleEI() {
    return EI;
}

QString Fit::getei() {
    QString str;
    if (shaftNotDefined)
        str = "  undefined";
    else if (shaftReady()) {
        str.setNum(ei);
        str.insert(0, "  ");
        str.append(QString::fromUtf8(" µm"));
    }
    else
        str = "";
    return str;
}

double Fit::getDoublees() {
    return es;
}

QString Fit::getes() {
    QString str;
    if (shaftNotDefined)
        str = "  undefined";
    else if (shaftReady()) {
        str.setNum(es);
        str.insert(0, "  ");
        str.append(QString::fromUtf8(" µm"));
    }
    else
        str = "";
    return str;
}

double Fit::getDoubleei() {
    return ei;
}

QString Fit::getGoB() {
    QString str;
    if (holeNotDefined)
        str = "  undefined";
    else if (holeReady()) {
        str.setNum(GoB);
        str.insert(0, "  ");
        str.append(" mm");
    }
    else
        str = "";
    return str;
}

QString Fit::getGuB() {
    QString str;
    if (holeNotDefined)
        str = "  undefined";
    else if (holeReady()) {
        str.setNum(GuB);
        str.insert(0, "  ");
        str.append(" mm");
    }
    else
        str = "";
    return str;
}

QString Fit::getGoW() {
    QString str;
    if (shaftNotDefined)
        str = "  undefined";
    else if (shaftReady()) {
        str.setNum(GoW);
        str.insert(0, "  ");
        str.append(" mm");
    }
    else
        str = "";
    return str;
}

QString Fit::getGuW() {
    QString str;
    if (shaftNotDefined)
        str = "  undefined";
    else if (shaftReady()) {
        str.setNum(GuW);
        str.insert(0, "  ");
        str.append(" mm");
    }
    else
        str = "";
    return str;
}

QString Fit::getFitKind() {
    QString str("");
    if (fitReady) {
        str.append("  ");
        str.append(fitKind);
    }
    return str;
}

QString Fit::getFitDescriptionMax() {
    if (fitKind == "Spiel" || fitKind == QString::fromUtf8("Übergang")) {
        return QString::fromUtf8(" Höchstspiel <b>[P<sub>SH</sub>]</b> ");
    } else if (fitKind == QString::fromUtf8("Übermaß")) {
        return QString::fromUtf8(" Höchstübermaß <b>[P<sub>ÜH</sub>]</b> ");
    } else {
        return QString("");
    }
}

QString Fit::getFitDescriptionMin() {
    if (fitKind == "Spiel") {
        return QString::fromUtf8(" Mindestspiel <b>[P<sub>SM</sub>]</b> ");
    } else if (fitKind == QString::fromUtf8("Übergang")) {
        return QString::fromUtf8(" Höchstübermaß <b>[P<sub>ÜH</sub>]</b> ");
    } else if (fitKind == QString::fromUtf8("Übermaß")) {
        return QString::fromUtf8(" Mindestübermaß <b>[P<sub>ÜM</sub>]</b> ");
    } else {
        return QString("");
    }
}

QString Fit::getFitMax() {
    QString str;
    //if ((holeNotDefined || shaftNotDefined) && !(holeReady() && shaftReady())) {
    if (fitReady) {
        if (fitKind == QString::fromUtf8("Übermaß")) {
            str.setNum(qAbs(minP));
        } else {
            str.setNum(qAbs(maxP));
        }
        str.insert(0, "  ");
        str.append(QString::fromUtf8(" µm"));
    } else {
        str = "";
    }
    return str;
}

QString Fit::getFitMin() {
    QString str;
    //if ((holeNotDefined || shaftNotDefined) && !(holeReady() && shaftReady())) {
    if (fitReady) {
        if (fitKind == QString::fromUtf8("Übermaß")) {
            str.setNum(qAbs(maxP));
        } else {
            str.setNum(qAbs(minP));
        }
        str.insert(0, "  ");
        str.append(QString::fromUtf8(" µm"));
    } else {
        str = "";
    }
    return str;
}

void Fit::setHole() {
    for (int i = 0; i < fundamentalTol.size(); ++i) {
        if (NHole > fundamentalTol[i][0])
            continue;
        fundTolHole = fundamentalTol[i][tolGradeHole];
        break;
    }
    for (int i = 2; i < fundamentalDevHole.size(); ++i) {
        if (NHole > fundamentalDevHole[i][0].toDouble())
            continue;
        int columnStart = 0;
        int columnEnd = 0;
        for (int j = 1; j < fundamentalDevHole[1].size(); ++j) {
            if (fundamentalDevHole[1][j] == tolZoneHole) {
                columnStart = j;
                break;
            }
        }
        for (columnEnd = columnStart; columnEnd < fundamentalDevHole[1].size(); ++columnEnd) {
            if (fundamentalDevHole[1][columnEnd] != tolZoneHole) {
                --columnEnd;
                break;
            }
        }
        int column = columnStart;
        for (; column < columnEnd; ++column) {
            QStringList range = fundamentalDevHole[0][column].split("-");
            if (range.size() == 1 && tolGradeHole == range[0].toInt()) {
                break;
            } else if (range.size() == 1 && tolGradeHole > range[0].toInt()) {
                continue;
            } else if (range.size() == 2){
                if (tolGradeHole >= range[0].toInt() && tolGradeHole <= range[1].toInt()) {
                    break;
                }
            }
        }
        fundDevHole = fundamentalDevHole[i][column];
        break;
    }
}

void Fit::setShaft() {
    // Grundtoleranz
    for (int i = 0; i < fundamentalTol.size(); ++i) {
        if (NShaft > fundamentalTol[i][0])
            continue;
        fundTolShaft = fundamentalTol[i][tolGradeShaft];
        break;
    }
    // Grundabmasz
    for (int i = 2; i < fundamentalDevShaft.size(); ++i) {
        // row
        if (NShaft > fundamentalDevShaft[i][0].toDouble())
            continue;
        // column
        // Toleranzfeld
        int columnStart = 0;
        int columnEnd = 0;
        for (int j = 1; j < fundamentalDevShaft[1].size(); ++j) {
            if (fundamentalDevShaft[1][j] == tolZoneShaft) {
                columnStart = j;
                break;
            }
        }
        // Spaltenende des Toleranzfeldes
        for (columnEnd = columnStart; columnEnd < fundamentalDevShaft[1].size(); ++columnEnd) {
            if (fundamentalDevShaft[1][columnEnd] != tolZoneShaft) {
                --columnEnd;
                break;
            }
        }
        int column = columnStart;
        for (; column < columnEnd; ++column) {
            QStringList range = fundamentalDevShaft[0][column].split("-");
            if (range.size() == 1 && tolGradeShaft == range[0].toInt()) {
                break;
            } else if (range.size() == 1 && tolGradeShaft > range[0].toInt()) {
                continue;
            } else if (range.size() == 2){
                if (tolGradeShaft >= range[0].toInt() && tolGradeShaft <= range[1].toInt()) {
                    break;
                }
            }
        }
        fundDevShaft = fundamentalDevShaft[i][column];
        break;
    }
}

void Fit::delSetHole() {
    fundTolHole = 0;
    fundDevHole = "";
    ES = 0;
    EI = 0;
    GoB = 0;
    GuB = 0;
    delFit();
}

void Fit::delSetShaft() {
    fundTolShaft = 0;
    fundDevShaft = "";
    es = 0;
    ei = 0;
    GoW = 0;
    GuW = 0;
    delFit();
}

void Fit::delFit() {
    fitKind = "";
    maxP = 0;
    minP = 0;
    fitReady = 0;
}

void Fit::debugOutput() {
    return;
    QDebug dbg = qDebug();
    dbg << "NHole =" << NHole << " NShaft =" << NShaft;
    dbg << "  tolZoneHole =" << tolZoneHole << "tolZoneShaft =" << tolZoneShaft;
    dbg << "  tolGradeHole =" << tolGradeHole << "tolGradeShaft =" << tolGradeShaft;
    //dbg << "  complete =" << complete;
    dbg << "  fundTolHole =" << fundTolHole;
    dbg << "  fundTolShaft =" << fundTolShaft;
    //dbg << "  fundamentalDevHole =" << fundamentalDevHole[6][1];
    //dbg << "  fundDevHole =" << fundDevHole;
    //dbg << "  fundDevShaft =" << fundDevShaft;
    dbg << "  ES =" << ES;
    dbg << "  EI =" << EI;
    dbg << "  es =" << es;
    dbg << "  ei =" << ei;
}

int Fit::calcHole() {
    if (fundDevHole == QString("u")) {
        delSetHole();
        holeNotDefined = 1;
    } else {
        if (fundDevHole == QString("JS")) {
            EI = (fundTolHole / 2.0) * (-1.0);
            ES = fundTolHole / 2.0;
        } else  if (tolZoneHole > QString("H")) {
            EI = fundDevHole.toDouble() - fundTolHole;
            //EI = fundDevHole.toDouble();
            ES = fundDevHole.toDouble();
        } else {
            EI = fundDevHole.toDouble();
            //ES = fundDevHole.toDouble();
            ES = fundDevHole.toDouble() + fundTolHole;
        }
        GoB = NHole + ES / 1000.0;
        GuB = NHole + EI / 1000.0;
        holeNotDefined = 0;
        if (shaftReady())
            calcFit();
    }
    return 0;
}

int Fit::calcShaft() {
    if (fundDevShaft == QString("u")) {
        delSetShaft();
        shaftNotDefined = 1;
    } else {
        if (fundDevShaft == QString("js")) {
            ei = (fundTolShaft / 2.0) * (-1.0);
            es = fundTolShaft / 2.0;
        } else if (tolZoneShaft > QString("h")){
            ei = fundDevShaft.toDouble();
            es = fundDevShaft.toDouble() + fundTolShaft;
        } else {
            ei = fundDevShaft.toDouble() - fundTolShaft;
            es = fundDevShaft.toDouble();
        }
        GoW = NShaft + es / 1000.0;
        GuW = NShaft + ei / 1000.0;
        shaftNotDefined = 0;
        if (holeReady())
            calcFit();
    }
    return 0;
}

int Fit::calcFit() {
    if (holeNotDefined || shaftNotDefined)
        return 1;
    maxP = (GoB - GuW) * 1000.0;
    minP = (GuB - GoW) * 1000.0;
    if (maxP > 0 && minP >= 0)
        fitKind = "Spiel";
    else if (maxP > 0 && minP < 0)
        fitKind = QString::fromUtf8("Übergang");
    else
        fitKind = QString::fromUtf8("Übermaß");
    fitReady = 1;
    return 0;
}

void Fit::PrintFundamentalTolToStd() {
    QDebug dbg = qDebug();
    dbg << "fundamental tolerances" << "\n";
    for (int i = 0; i < fundamentalTol.size(); ++i) {
        for (int j = 0; j < fundamentalTol[i].size(); ++j) {
            dbg << fundamentalTol[i][j] << "  ";
        }
        dbg << "\n";
    }
}

void Fit::PrintFundamentalDevHoleToStd() {
    QDebug dbg = qDebug();
    dbg << "fundamental deviations (hole)" << "\n";
    //for (int i = 0; i < fundamentalDevHole.size(); ++i) {
    for (int i = 0; i < 5; ++i) {
        //for (int j = 0; j < fundamentalDevHole[i].size(); ++j) {
        for (int j = 0; j < 10; ++j) {
            dbg << fundamentalDevHole[i][j] << "  ";
        }
        dbg << "\n";
    }
}

void Fit::PrintFundamentalDevShaftToStd() {
    QDebug dbg = qDebug();
    dbg << "fundamental deviations (shaft)" << "\n";
    //for (int i = 0; i < fundamentalDevShaft.size(); ++i) {
    for (int i = 0; i < 5; ++i) {
        //for (int j = 0; j < fundamentalDevShaft[i].size(); ++j) {
        for (int j = 0; j < 10; ++j) {
            dbg << fundamentalDevShaft[i][j] << "  ";
        }
        dbg << "\n";
    }
}
