#include "server.h"

Server::Server(QWidget *parent)
    : QMainWindow(parent)
{
    buildGui();
}

Server::~Server()
{
    delete listOfClients;
    delete blockSizeList;
}

void Server::startServerServices()
{
    if (!server->listen(QHostAddress::Any, portEdit->text().toInt())) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    } else {
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                    ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }

        if (ipAddress.isEmpty())
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        serverStatus->setText(tr("The server is running on\n\nIP: %1 port: %2\n")
                             .arg(ipAddress).arg(server->serverPort()));
        stopButton->setEnabled(true);
        startButton->setEnabled(false);
    }
}

void Server::stopServerServices()
{
    server->close();
    serverStatus->setText("Server's closed");
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}

void Server::quitfromProgram()
{
    server->close();
    close();
}

void Server::addNewClient()
{
    QTcpSocket *client = server->nextPendingConnection();
    listOfClients->push_back(client);
    blockSizeList->push_back(0);

    connect(client, SIGNAL(disconnected()), this, SLOT(removeClient()));
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));
    connect(client, SIGNAL(readyRead()), this, SLOT(sendMessageForward()));
}

void Server::removeClient()
{
    QTcpSocket *client = (QTcpSocket*) sender();

    int index = listOfClients->indexOf(client);
    listOfClients->removeAt(index);
    blockSizeList->removeAt(index);
}

void Server::sendMessageForward()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    int index = listOfClients->indexOf(client);

    QDataStream in(client);
    in.setVersion(QDataStream::Qt_4_6);

    if (blockSizeList->at(index) == 0)
    {
        if (client->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }

        quint16 sizeOfBlock;
        in >> sizeOfBlock;
        (*blockSizeList)[index] = sizeOfBlock;
    }

    if (client->bytesAvailable() < blockSizeList->at(index))
    {
        return;
    }

    QString nick;
    in >> nick;

    QString message;
    in >> message;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << blockSizeList->at(index);
    out << nick;
    out << message;

    for (int i = 0; i < listOfClients->size(); ++i)
    {
        if (i != index)
        {
            listOfClients->at(i)->write(data);
        }
    }

    (*blockSizeList)[index] = (quint16)0;
}

void Server::buildGui()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(addNewClient()));

    listOfClients = new QList<QTcpSocket*>();
    blockSizeList = new QList<quint16>();

    this->setGeometry(50, 50, 200, 300);
    this->setStyleSheet("background-color:black;");
    this->setFixedSize(200, 300);

    port = new QLabel("<font color='Green'>Port: </font>", this);
    port->setGeometry(10, 20, 30, 30);
    setFont(port, 8);

    portEdit = new QLineEdit(this);
    portEdit->setGeometry(50, 25, 120, 20);
    palette = new QPalette();
    palette->setColor(QPalette::Text, Qt::green);
    portEdit->setPalette(*palette);

    serverStatus = new QLabel("Status: ", this);
    serverStatus->setGeometry(10, 50, 180, 50);
    setFont(serverStatus, 8);

    startButton = new QPushButton("Start", this);
    startButton->setGeometry(10, 190, 180, 30);
    setFont(startButton, 10);
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startServerServices()));

    stopButton = new QPushButton("Stop", this);
    stopButton->setGeometry(10, 220, 180, 30);
    stopButton->setEnabled(false);
    setFont(stopButton, 10);
    connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopServerServices()));

    quitButton = new QPushButton("Quit", this);
    quitButton->setGeometry(10, 250, 180, 30);
    setFont(quitButton, 10);
    connect(quitButton, SIGNAL(clicked(bool)), this, SLOT(quitfromProgram()));
}

void Server::setFont(QLabel *label, quint16 fontSize)
{
    font = label->font();
    font.setPointSize(fontSize);
    font.setBold(true);
    label->setFont(font);
}

void Server::setFont(QPushButton *button, quint16 fontSize)
{
    font = button->font();
    font.setPointSize(fontSize);
    font.setBold(true);
    button->setFont(font);
}

