#include <QtGui/QApplication>
#include "fitview.h"
#include "fit.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Daten);

    QApplication a(argc, argv);
    fitView *w = new fitView;
    w->show();
    return a.exec();
}
