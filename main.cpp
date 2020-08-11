
#include "TopPainterWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TopPainterWidget topPainter;
    topPainter.show();

    return a.exec();
}
