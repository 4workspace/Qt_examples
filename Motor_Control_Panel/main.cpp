#include "dialog.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("Motor Control Panel");
    w.setWindowIcon(QIcon("C:\\Users\\Point\\Desktop\\Qt\\TargetApp\\ICON\\ico.png"));

    w.show();
    return a.exec();
}
