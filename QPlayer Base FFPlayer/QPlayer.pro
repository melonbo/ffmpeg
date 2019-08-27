#-------------------------------------------------
#
# Project created by QtCreator 2019-07-04T14:53:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -fpermissive

INCLUDEPATH += $$PWD/ffmpeg/include $$PWD/sdl/include

#LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
LIBS += $$PWD/ffmpeg/lib/libavutil.dll.a \
        $$PWD/ffmpeg/lib/libavcodec.dll.a \
        $$PWD/ffmpeg/lib/libavdevice.dll.a \
        $$PWD/ffmpeg/lib/libavfilter.dll.a \
        $$PWD/ffmpeg/lib/libavformat.dll.a \
        $$PWD/ffmpeg/lib/libpostproc.dll.a \
        $$PWD/ffmpeg/lib/libswresample.dll.a \
        $$PWD/ffmpeg/lib/libswscale.dll.a


LIBS += -L$$PWD/sdl/lib -lSDL2

SOURCES += \
        main.cpp \
        QPlayer.cpp \
        demux.cpp \
    cmdutils.c

HEADERS += \
        QPlayer.h \
        pch.h \
        demux.h \
    config.h \
    cmdutils.h \
    ffmpeg.h

FORMS += \
        QPlayer.ui

DISTFILES += \
    记录.text
