#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class TCPClient;
}
QT_END_NAMESPACE

class TCPClient : public QMainWindow
{
    Q_OBJECT

public:
    TCPClient(QWidget *parent = nullptr);
    ~TCPClient();

public slots:
    void slotReadyRead();

private:
    Ui::TCPClient *ui;
    QTcpSocket *socket;
    QByteArray Data;
    quint16 nextBlockSize = 0;                      /// переменная для хранения размера блока
    void sendToServer(QString message);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
};
#endif // TCPCLIENT_H
