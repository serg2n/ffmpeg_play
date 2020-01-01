#include "fmplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FmPlayer w;
    w.show();

    return a.exec();
}
