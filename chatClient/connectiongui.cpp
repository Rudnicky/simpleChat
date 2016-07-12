#include "connectiongui.h"

#include <QGridLayout>

ConnectionGUI::ConnectionGUI(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    buildGui();
}

void ConnectionGUI::connectToHost()
{
    emit connectToHost(hostEdit->text(), portEdit->text().toInt(), nickEdit->text());
    close();
}

void ConnectionGUI::buildGui()
{
    nickEdit = new QLineEdit(this);
    hostEdit = new QLineEdit(this);

    portEdit = new QLineEdit(this);
    portEdit->setValidator(new QIntValidator(1, 65535, this));

    nick = new QLabel("Nick: ", this);
    nick->setBuddy(nickEdit);

    host = new QLabel("Server: ", this);
    host->setBuddy(hostEdit);

    port = new QLabel("Port: ", this);
    port->setBuddy(portEdit);

    connectButton = new QPushButton("Connect", this);
    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(connectToHost()));

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(host, 0, 0, 1, 1);
    mainLayout->addWidget(hostEdit, 0, 1, 1, 2);
    mainLayout->addWidget(port, 1, 0, 1, 1);
    mainLayout->addWidget(portEdit, 1, 1, 1, 2);
    mainLayout->addWidget(nick, 2, 0, 1, 1);
    mainLayout->addWidget(nickEdit, 2, 1, 1, 2);
    mainLayout->addWidget(connectButton, 3, 2, 1, 1);
}
