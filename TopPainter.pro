
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += static

TARGET = TopPainter
#CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS = -static
QMAKE_LFLAGS += -static

TEMPLATE = app

SOURCES += main.cpp \
    TopPainterWidget.cpp \
    ImageButton.cpp \
    XGlobalHook.cpp \
    ToolBarWidget.cpp

HEADERS += \
    TopPainterWidget.h \
    ImageButton.h \
    XGlobalHook.h \ 
    ToolBarWidget.h \
    ICmd.h
