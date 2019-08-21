#-------------------------------------------------
#
# Project created by QtCreator 2019-04-11T13:41:49
# Based on QT5.10.0
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FFSaveFile
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/ffmpeg/include
LIBS += $$PWD/ffmpeg/lib/libavcodec.dll.a \
        $$PWD/ffmpeg/lib/libavfilter.dll.a \
        $$PWD/ffmpeg/lib/libavformat.dll.a \
        $$PWD/ffmpeg/lib/libswscale.dll.a \
        $$PWD/ffmpeg/lib/libavutil.dll.a \

SOURCES += main.cpp

HEADERS += \
    pch.h
