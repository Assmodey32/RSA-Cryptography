#include "FrontendRSA.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FrontendRSA w;
    w.show();
    return a.exec();
}
