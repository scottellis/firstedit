#include "firstedit.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FirstEdit w;

    w.show();

    return a.exec();
}
