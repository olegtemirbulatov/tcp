#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QList>

class TCPServer : public QTcpServer
{
    Q_OBJECT

public:
    TCPServer();

private:
    QList<QTcpSocket*> connectedSockets;                /// Сокеты, в которые будет проводиться рассылка
    QByteArray Data;                                    /// Данные, которые будут передаваться между клиентом и сервером
    void sendToClient();

public slots:
    void incomingConnection(qintptr socketDescriptor);  /// Слот для обработки подключений к серверу
    void socketDisconnection();
};

#endif // TCPSERVER_H
