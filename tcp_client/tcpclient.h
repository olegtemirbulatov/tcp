#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QPushButton>
#include <QTextBrowser>

class TCPClient : public QWidget
{
    Q_OBJECT

public:
    TCPClient(QWidget *parent = nullptr);
    ~TCPClient();

public slots:
    void slotReadyRead();

private:
    QVBoxLayout *mainLayout;
    QPushButton *connectButton;
    QTextBrowser *textBrowser;
    QTcpSocket *socket;
    QByteArray Data;
    quint16 nextBlockSize = 0;      /// переменная для хранения размера блока

private slots:
    void on_connectButton_clicked();
};

#endif // TCPCLIENT_H
