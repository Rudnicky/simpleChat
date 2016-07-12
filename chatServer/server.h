#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QFont>

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private slots:
    void startServerServices();
    void stopServerServices();
    void quitfromProgram();
    void addNewClient();
    void removeClient();
    void sendMessageForward();

private:
    void buildGui();
    void setFont(QLabel *label, quint16 fontSize);
    void setFont(QPushButton *button, quint16 fontSize);

    QList<QTcpSocket*> *listOfClients;
    QList<quint16> *blockSizeList;
    QTcpServer *server;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *quitButton;
    QLabel *serverStatus;
    QLabel *port;
    QLineEdit *portEdit;
    QFont font;
    QPalette *palette;

};

#endif // SERVER_H
