#ifndef FIT_H
#define FIT_H

#include <QObject>
#include <QVector>
#include <QDebug>

class Fit : public QObject
{
Q_OBJECT
public:
    explicit Fit(QObject *parent = 0);
    void PrintFundamentalTolToStd();
    void PrintFundamentalDevHoleToStd();
    void PrintFundamentalDevShaftToStd();

    QString getES();
    double getDoubleES();
    QString getEI();
    double getDoubleEI();
    QString getei();
    double getDoubleei();
    QString getes();
    double getDoublees();
    QString getGoB();
    QString getGuB();
    QString getGoW();
    QString getGuW();
    QString getFitKind();
    QString getFitDescriptionMax();
    QString getFitDescriptionMin();
    QString getFitMax();
    QString getFitMin();

    bool isHoleDefined();
    bool isShaftDefined();

    bool isFitReady() {
        return fitReady;
    }

    bool holeReady() {
        return (NHole && !tolZoneHole.isEmpty() && tolGradeHole > 0 && tolGradeHole <= 18);
    }

    bool shaftReady() {
        return (NShaft && !tolZoneShaft.isEmpty() && tolGradeShaft > 0 && tolGradeShaft <= 18);
    }

signals:
    void holeChanged();
    void shaftChanged();

public slots:
    void setNHole(double value);
    void setNShaft(double value);
    void setTolZoneHole(QString s);
    void setTolZoneShaft(QString s);
    void setTolGradeHole(QString s);
    void setTolGradeShaft(QString s);
    void valueChanged();

private:
    void setHole();
    void setShaft();
    void delSetHole();
    void delSetShaft();
    void delFit();
    void debugOutput();
    int calcHole();
    int calcShaft();
    bool holeNotDefined;
    bool shaftNotDefined;
    int calcFit();
    bool fitReady;

    double fundTolHole;
    double fundTolShaft;

    QString fundDevHole;
    QString fundDevShaft;

    QVector<QVector<double> > fundamentalTol;
    QVector<QVector<QString> > fundamentalDevHole;
    QVector<QVector<QString> > fundamentalDevShaft;
    QString qsFit;      // Passungsart
    //double N;      // N - Nennmasz
    double NHole;   // NHole - Nennmasz Bohrung
    double NShaft;  // NShaft - Nennmasz Welle

    double GoB;     // GoB - Hoechstmasz Bohrung
    double GuB;     // GuB - Mindestmasz Bohrung
    double ES;      // ES - oberes Abmasz Bohrung
    double EI;      // EI - unteres Abmasz Bohrung

    double GoW;     // GoW - Hoechstmasz Welle
    double GuW;     // GuW - Mindestmasz Welle
    double es;      // oberes Abmasz Welle
    double ei;      // unteres Abmasz Welle

    double maxP;
    double minP;

    QString fitKind;

    QString tolZoneHole;
    QString tolZoneShaft;
    int tolGradeHole;
    int tolGradeShaft;

};

#endif // FIT_H
