#-------------------------------------------------
#
# Project created by QtCreator 2016-02-14T11:34:05
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGifPlayer
TEMPLATE = app

CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
    DEFINES += DEBUG
    #QMAKE_CXXFLAGS += -O3 #-std=c++11
}
else{
    CONFIG -= debug release
    CONFIG += release
    DEFINES += QT_NO_DEBUG_OUTPUT
    #QMAKE_CXXFLAGS += -O3 #-std=c++11
}

SOURCES += main.cpp\
        widget.cpp \
    libnsgif/libnsgif.c \
    libnsgif/gif.cpp \
    imageview.cpp

HEADERS  += widget.h \
    libnsgif/libnsgif.h \
    libnsgif/gif.h \
    logdef.h \
    imageview.h

FORMS    += widget.ui

LIBS += -lglut -lGLU
