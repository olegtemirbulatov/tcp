#include "tcpclient.h"

TCPClient::TCPClient(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    connectButton = new QPushButton("Connect to server");
    textBrowser = new QTextBrowser();
    connectButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    textBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(connectButton);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);
    resize(400, 500);

    socket = new QTcpSocket(this);
    socket->open(QIODevice::ReadOnly);

    connect(connectButton, &QPushButton::clicked, this, &TCPClient::on_connectButton_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &TCPClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

TCPClient::~TCPClient()
{
    socket->close();
}

void TCPClient::slotReadyRead()
{
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_5_15);
    if (input.status() == QDataStream::Ok)
    {
        while (true)
        {
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)
                {
                    break;
                }
                input >> nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize)   // данные пришли не полностью
            {
                break;
            }

            // если мы все еще не вышли из цикла
            QString message;
            input >> message;
            nextBlockSize = 0;      // обнуляем, чтобы можно было принимать новые сообщения
            textBrowser->append(message);
        }
    }
    else
    {
        textBrowser->append("Error while reading socket");
    }
}

void TCPClient::on_connectButton_clicked()
{
    socket->connectToHost("127.0.0.1", 1234);
}
