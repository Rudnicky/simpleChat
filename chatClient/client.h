#ifndef CLIENT_H
#define CLIENT_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QTcpSocket>
#include <QMenuBar>
#include <QDataStream>
#include <QAbstractSocket>
#include <QAction>
#include <QHostAddress>
#include <QMessageBox>
#include <QGridLayout>
#include <QFont>
#include "connectiongui.h"

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);
    ~Client();

private slots:
    void connectToServer(QString, int, QString);
    void disconnectFromServer();
    void isConnectedToServer();
    void showWindowOfConnection();
    void sendMessage();
    void exitProgram();
    void readData();
    void possibleErrors(QAbstractSocket::SocketError);

private:
    QMenu *menu;
    QAction *connectToServerAction;
    QAction *disconnectFromServerAction;
    QAction *quitProgramAction;

    QTcpSocket *socket;
    QTextBrowser *chat;
    QPushButton *sendMessageButton;
    QLineEdit *messageEdit;
    QLabel *nick;
    QLabel *chatRoomLabel;
    QString nickname;
    quint16 blockSize;

    void menuTool();
    void buildGui();
};

#endif // CLIENT_H
