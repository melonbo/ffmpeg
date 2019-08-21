TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    pch.h

QMAKE_CXXFLAGS += -fpermissive
INCLUDEPATH += $$PWD/ffmpeg/include
LIBS += $$PWD/ffmpeg/lib/libavcodec.dll.a \
        $$PWD/ffmpeg/lib/libavfilter.dll.a \
        $$PWD/ffmpeg/lib/libavformat.dll.a \
        $$PWD/ffmpeg/lib/libswscale.dll.a \
        $$PWD/ffmpeg/lib/libavutil.dll.a \
        $$PWD/ffmpeg/lib/libavdevice.dll.a

DISTFILES += \
    readme.txt
