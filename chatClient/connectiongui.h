#ifndef CONNECTIONGUI_H
#define CONNECTIONGUI_H

#include <QWidget>
#include <QValidator>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>


class ConnectionGUI : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionGUI(QWidget *parent = 0, Qt::WindowFlags f = 0);

private slots:
    void connectToHost();

signals:
    void connectToHost(QString, int, QString);

private:
    QLabel *host;
    QLabel *port;
    QLabel *nick;
    QLineEdit *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *nickEdit;
    QPushButton *connectButton;

    void buildGui();
};

#endif // CONNECTIONGUI_H
