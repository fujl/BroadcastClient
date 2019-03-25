#ifndef BROADCASTCLIENTDLG_H
#define BROADCASTCLIENTDLG_H

#include <QtWidgets>
#include <QUdpSocket>

class BroadcastClientDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BroadcastClientDlg(QWidget *parent = 0);
    ~BroadcastClientDlg();

private slots:
    void clickedConnectButton();
    void clickedSendButton();
    void recvData();
private:
    QLabel *ipaddressLabel;
    QLineEdit *ipaddressLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QTextEdit *sendText;
    QTextEdit *recvText;
    QPushButton *connectButton;
    QPushButton *sendButton;
    QPushButton *quitButton;

    QUdpSocket *udpSocket;
};

#endif // BROADCASTCLIENTDLG_H
