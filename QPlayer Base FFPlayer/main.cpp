#include "QPlayer.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include "demux.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPlayer w;
    w.show();
    return a.exec();
}
