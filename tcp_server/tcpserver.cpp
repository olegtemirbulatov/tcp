#include <QDataStream>
#include <algorithm>

#include "tcpserver.h"

TCPServer::TCPServer()
{
    if (this->listen(QHostAddress::Any, 8080))
    {
        qDebug() << "Server is running";
    }
    else
    {
        qDebug() << "Server is not started";
    }
}

void TCPServer::sendToClient()
{
    Data.clear();
    QDataStream output(&Data, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_15);
    QString message = "Текущее число TCP-клиентов: " + QString::number(connectedSockets.size());
    output << quint16(0) << message;                        // резервируем первые 2 байта под размер сообщения
    output.device()->seek(0);                               // Переходим в самое начало блока
    output << quint16(Data.size() - sizeof(quint16));       // записываем размер блока в первые 2 байта передаваемого сообщения
    for (auto it = connectedSockets.cbegin(); it != connectedSockets.cend(); ++it)
    {
        (*it)->write(Data);
    }
}

void TCPServer::incomingConnection(const qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);          // socketDescriptor - unsigned, которое идентифицирует поток ввода-вывода
    socket->open(QIODevice::WriteOnly);
    connect(socket, &QTcpSocket::disconnected, this, &TCPServer::socketDisconnection);

    connectedSockets.push_back(socket);
    qDebug() << "Client connected " << socketDescriptor;
    sendToClient();
}

void TCPServer::socketDisconnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    for (int i = 0; i < connectedSockets.size(); ++i)
    {
        if (connectedSockets.at(i) == socket)
        {
            connectedSockets.removeAt(i);
            break;
        }
    }
    socket->close();
    socket->deleteLater();
    sendToClient();
}

