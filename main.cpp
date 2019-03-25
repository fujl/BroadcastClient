#include "broadcastclientdlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BroadcastClientDlg w;
    w.show();

    return a.exec();
}
