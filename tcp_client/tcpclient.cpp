#include "tcpclient.h"
#include "ui_tcpclient.h"

TCPClient::TCPClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TCPClient)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &TCPClient::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

TCPClient::~TCPClient()
{
    delete ui;
}

void TCPClient::slotReadyRead()
{
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_5_15); // потом поменять
    if (input.status() == QDataStream::Ok)
    {
        while (true)
        {
            if (nextBlockSize == 0)
            {
                if (socket->bytesAvailable() < 2)           // почитать как работает
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
            nextBlockSize = 0;                              // обнуляем, чтобы можно было принимать новые сообщения
            ui->textBrowser->append(message);
        }
    }
    else
    {
        ui->textBrowser->append("Error while reading socket");
    }
}

void TCPClient::sendToServer(QString message)
{
    Data.clear();
    QDataStream output(&Data, QIODevice::WriteOnly);        // почитать как работает
    output.setVersion(QDataStream::Qt_5_15);                // изменить версию, если получится подгрузить qt6
    output << quint16(0) << message;
    output.device()->seek(0);                               // Переходим в самое начало блока
    output << quint16(Data.size() - sizeof(quint16));       // записываем размер блока в первые 2 байта передаваемого сообщения
    socket->write(Data);
    ui->lineEdit->clear();
}

void TCPClient::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 2323); // потом поменять адрес порта
}


void TCPClient::on_pushButton_2_clicked()
{
    sendToServer(ui->lineEdit->text());
}


void TCPClient::on_lineEdit_returnPressed()
{
    sendToServer(ui->lineEdit->text());
}

