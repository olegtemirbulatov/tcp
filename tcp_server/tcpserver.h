#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QVector>

class TCPServer : public QTcpServer
{
    Q_OBJECT

public:
    TCPServer();
    ~TCPServer() = default;
    QTcpSocket *socket;                     /// Сокет, который создается для каждого нового подключения

private:
    QVector<QTcpSocket*> socketsVector;     /// Сокеты, в которые будет проводиться рассылка
    QByteArray Data;                        /// Данные, которые будут передаваться между клиентом и сервером
    quint16 nextBlockSize = 0;              /// переменная для хранения размера блока
    void sendToClient();

public slots:
    void incomingConnection(qintptr socketDescriptor);  /// Слот для обработки подключений к серверу
    void socketDisconnection();
};

#endif // TCPSERVER_H
