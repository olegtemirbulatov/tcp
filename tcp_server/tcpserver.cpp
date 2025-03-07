#include <QDataStream>
#include <algorithm>

#include "tcpserver.h"

TCPServer::TCPServer()
{
    if (this->listen(QHostAddress::Any, 2323)) // rewrite port number
    {
        qDebug() << "Server is running";
    }
    else
    {
        qDebug() << "Error while starting the server";
    }
}

void TCPServer::sendToClient(QString message)
{
    Data.clear();
    QDataStream output(&Data, QIODevice::WriteOnly);        // почитать как работает
    output.setVersion(QDataStream::Qt_5_15);                // изменить версию, если получится подгрузить qt6
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
    socket->setSocketDescriptor(socketDescriptor);                                          // socketDescriptor - unsigned которое идентифицирует поток ввода-вывода
    connect(socket, &QTcpSocket::readyRead, this, &TCPServer::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TCPServer::socketDisconnection);

    socketsVector.push_back(socket);
    qDebug() << "Client connected " << socketDescriptor;
    socket = nullptr;
}

void TCPServer::socketDisconnection()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    socketsVector.erase(std::remove(socketsVector.begin(), socketsVector.end(), socket), socketsVector.end());
    socket->deleteLater();
}

void TCPServer::slotReadyRead()
{
    socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_5_15); // изменить версию, если получится подгрузить qt6
    if (input.status() == QDataStream::Ok)
    {
        qDebug() << "read...";
        while (true)
        {
            if (nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize = 0";
                if (socket->bytesAvailable() < 2)           // почитать как работает
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                input >> nextBlockSize;
                qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)   // данные пришли не полностью
            {
                qDebug() << "Data not full, break";
                break;
            }

            // если мы все еще не вышли из цикла
            QString message;
            input >> message;
            nextBlockSize = 0;                              // обнуляем, чтобы можно было принимать новые сообщения
            qDebug() << message;
            sendToClient(message);
        }
    }
    else
    {
        qDebug() << "DataStream error!";
    }
}

