#include "broadcastclientdlg.h"
#include "ui_broadcastclientdlg.h"
#include <QMessageBox>

BroadcastClientDlg::BroadcastClientDlg(QWidget *parent) :
    QDialog(parent)
{
    ipaddressLabel = new QLabel(tr("IP:"), this);
    portLabel = new QLabel(tr("Port:"), this);

    ipaddressLineEdit = new QLineEdit(this);
    ipaddressLineEdit->setPlaceholderText(tr("请输入IP地址"));
    portLineEdit = new QLineEdit(this);
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    portLineEdit->setPlaceholderText(tr("请输入端口"));

    sendText = new QTextEdit(this);
    sendText->setPlaceholderText(tr("请输入发送内容"));
    recvText = new QTextEdit(this);
    recvText->setReadOnly(true);

    connectButton = new QPushButton(tr("Connect"), this);
    sendButton = new QPushButton(tr("Send"), this);
    quitButton = new QPushButton(tr("Quit"), this);

    sendButton->setEnabled(false);

    QVBoxLayout *leftLayout = new QVBoxLayout;

    QHBoxLayout *ipaddressLayout = new QHBoxLayout;
    ipaddressLayout->addWidget(ipaddressLabel);
    ipaddressLayout->addWidget(ipaddressLineEdit);
    leftLayout->addLayout(ipaddressLayout);

    QHBoxLayout *portLayout = new QHBoxLayout;
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLineEdit);
    leftLayout->addLayout(portLayout);

    leftLayout->addWidget(sendText);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(quitButton);
    leftLayout->addLayout(buttonLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(recvText);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Client"));

    udpSocket = new QUdpSocket(this);

    connect(connectButton, SIGNAL(clicked()), this, SLOT(clickedConnectButton()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(clickedSendButton()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recvData()));
}

BroadcastClientDlg::~BroadcastClientDlg()
{

}

void BroadcastClientDlg::clickedConnectButton()
{
    if (portLineEdit->text() == "")
    {
        QMessageBox::information(this, tr("信息"), tr("请输入端口"));
        portLineEdit->setFocus();
        return;
    }
    quint16 port = portLineEdit->text().toInt();
    if (!udpSocket->bind(port)) {
        QMessageBox::information(this, tr("信息"), tr("绑定失败，端口已经被占用"));
        return;
    }
    portLineEdit->setEnabled(false);
    sendButton->setEnabled(true);
    connectButton->setEnabled(false);
}

void BroadcastClientDlg::clickedSendButton()
{
    QString strSendData = sendText->toPlainText();
    if (strSendData.isEmpty()) {
        QMessageBox::information(this, tr("信息"), tr("请输入发送内容"));
        return;
    }
    udpSocket->writeDatagram(strSendData.toLocal8Bit().data(), QHostAddress::Broadcast, portLineEdit->text().toInt());
}

QString IPV4IntegerToString(quint32 ip) {
    return QString("%1.%2.%3.%4").arg((ip>>24)&0xFF).arg((ip>>16)&0xFF).arg((ip>>8)&0xFF).arg(ip&0xFF);
}

void BroadcastClientDlg::recvData()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString recvBuffer = datagram.data();
        quint32 ip = sender.toIPv4Address();
        QString ipv4String = IPV4IntegerToString(ip);
        QString showString = QString("RECV %1' BROADCAST DATA:%2").arg(ipv4String).arg(recvBuffer);

        if (!ipaddressLineEdit->text().isEmpty()) {
            if (ipv4String == ipaddressLineEdit->text()) {
                recvText->append(showString);
            }
        }
        else
        {
            recvText->append(showString);
        }
    }
}
