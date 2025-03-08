#include <QDataStream>
#include <algorithm>

#include "tcpserver.h"

TCPServer::TCPServer()
{
    if (this->listen(QHostAddress::Any, 1234))
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
    QString message = "Текущее число TCP-клиентов: " + QString::number(socketsVector.size());
    output << quint16(0) << message;                        // резервируем первые 2 байта под размер сообщения
    output.device()->seek(0);                               // Переходим в самое начало блока
    output << quint16(Data.size() - sizeof(quint16));       // записываем размер блока в первые 2 байта передаваемого сообщения
    for (QVector<QTcpSocket*>::const_iterator it = socketsVector.begin(); it != socketsVector.end(); ++it)
    {
        (*it)->write(Data);
    }
}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);          // socketDescriptor - unsigned, которое идентифицирует поток ввода-вывода
    socket->open(QIODevice::WriteOnly);
    connect(socket, &QTcpSocket::disconnected, this, &TCPServer::socketDisconnection);

    socketsVector.push_back(socket);
    qDebug() << "Client connected " << socketDescriptor;
    sendToClient();
    socket = nullptr;
}

void TCPServer::socketDisconnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    socketsVector.erase(std::remove(socketsVector.begin(), socketsVector.end(), socket), socketsVector.end());
    socket->close();
    socket->deleteLater();
    sendToClient();
}

