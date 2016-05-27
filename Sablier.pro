#-------------------------------------------------
#
# Project created by QtCreator 2016-05-27T09:35:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sablier
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    task.cpp \
    event.cpp

HEADERS  += mainwindow.h \
    task.h \
    event.h \
    duration.h

LIBS     += -lical -licalss

FORMS    += mainwindow.ui
