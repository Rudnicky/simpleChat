#include "client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    buildGui();
}

Client::~Client()
{

}

void Client::connectToServer(QString ip, int port, QString nick)
{
    socket->abort();
    blockSize = 0;

    socket->connectToHost(ip, port);
    this->nickname = nick;
}

void Client::disconnectFromServer()
{
    socket->close();
    this->nickname.clear();
}

void Client::isConnectedToServer()
{
    chat->clear();
    chat->append(tr("U have connected to %1 on port %2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
    nick->setText(nickname + ':');
}

void Client::showWindowOfConnection()
{
    ConnectionGUI *connectionGUI = new ConnectionGUI(this, Qt::Window);
    connect(connectionGUI, SIGNAL(connectToHost(QString, int, QString)), this,
            SLOT(connectToServer(QString, int, QString)));
    connectionGUI->show();
}

void Client::sendMessage()
{
    chat->append(nickname + ": " + messageEdit->text());

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_6);
    out << (quint16) 0;

    out << nickname;

    out << messageEdit->text();

    out.device()->seek(0);

    out << (quint16) (data.size() - sizeof(quint16));

    socket->write(data);

    messageEdit->clear();
}

void Client::exitProgram()
{
    disconnectFromServer();
    close();
}

void Client::readData()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_6);

    if (blockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        {
            return;
        }

        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize)
    {
        return;
    }

    QString nick;
    in >> nick;

    QString message;
    in >> message;

    chat->append(nick + ": " + message);

    blockSize = 0;
}

void Client::possibleErrors(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;

    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                         tr("The host was not found. Please check the "
                                             "host name and port settings."));
        break;

    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                         tr("The connection was refused by the peer. "
                                             "Make sure the fortune server is running, "
                                             "and check that the host name and port "
                                             "settings are correct."));
        break;

    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                         tr("The following error occurred: %1.")
                                         .arg(socket->errorString()));
    }
}

void Client::menuTool()
{
    menu = menuBar()->addMenu(tr("Menu"));

    connectToServerAction = new QAction("Connect", this);
    connect(connectToServerAction, SIGNAL(triggered()), this, SLOT(showWindowOfConnection()));
    menu->addAction(connectToServerAction);

    disconnectFromServerAction = new QAction("Disconnect", this);
    connect(disconnectFromServerAction, SIGNAL(triggered()), this, SLOT(disconnectFromServer()));
    menu->addAction(disconnectFromServerAction);

    menu->addSeparator();

    quitProgramAction = new QAction("Exit", this);
    connect(quitProgramAction, SIGNAL(triggered()), this, SLOT(exitProgram()));
    menu->addAction(quitProgramAction);
}

void Client::buildGui()
{
    menuTool();
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(connected()), this, SLOT(isConnectedToServer()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(possibleErrors(QAbstractSocket::SocketError)));

    chat = new QTextBrowser(this);
    chat->setReadOnly(true);
    chat->setFocusPolicy(Qt::NoFocus);

    nick = new QLabel("<font color='Green'>Nick: </font>", this);

    messageEdit = new QLineEdit(this);
    connect(messageEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

    sendMessageButton = new QPushButton("Send", this);
    sendMessageButton->setDefault(true);
    sendMessageButton->setAutoDefault(true);
    connect(sendMessageButton, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));

    QWidget *mainWidget = new QWidget(this);
    mainWidget->setStyleSheet("background-color:black;");

    QGridLayout *mainLayout = new QGridLayout(mainWidget);
    mainLayout->addWidget(chat, 0, 0, 2, 3);
    mainLayout->addWidget(nick, 2, 0, 1, 1);
    mainLayout->addWidget(messageEdit, 2, 1, 1, 1);
    mainLayout->addWidget(sendMessageButton, 2, 2, 1, 1);
    setCentralWidget(mainWidget);

}

