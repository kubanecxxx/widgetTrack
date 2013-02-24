#-------------------------------------------------
#
# Project created by QtCreator 2013-02-21T17:47:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trackwidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widgettrack.cpp \
    widgetmultitrack.cpp

HEADERS  += mainwindow.h \
    widgettrack.h \
    widgetmultitrack.h

FORMS    += mainwindow.ui \
    widgettrack.ui \
    widgetmultitrack.ui

unix:!macx: LIBS += -lsndfile
