#-------------------------------------------------
#
# Project created by QtCreator 2016-07-11T12:45:25
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatClient
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    connectiongui.cpp

HEADERS  += client.h \
    connectiongui.h

FORMS    +=
