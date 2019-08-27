#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T16:10:47
#
#-------------------------------------------------

UI_DIR  = obj/Gui
MOC_DIR = obj/Moc
OBJECTS_DIR = obj/Obj


#������ļ�ֱ�ӷŵ�Դ��Ŀ¼�µ�binĿ¼�£���dll�������˴�Ŀ¼�У����Խ�����к��Ҳ���dll����
#DESTDIR=$$PWD/bin/
contains(QT_ARCH, i386) {
    message("32-bit")
    DESTDIR = $${PWD}/bin32
} else {
    message("64-bit")
    DESTDIR = $${PWD}/bin64
}
QMAKE_CXXFLAGS += -std=c++11


#������Ƶ�������Ĵ���
include($$PWD/module/VideoPlayer/VideoPlayer.pri)
#�������϶�����Ĵ���
include($$PWD/module/DragAbleWidget/DragAbleWidget.pri)

SOURCES += $$PWD/src/main.cpp \
    $$PWD/src/Widget/VideoPlayerWidget.cpp \
    $$PWD/src/Widget/ShowVideoWidget.cpp \
    $$PWD/src/Widget/VideoSlider.cpp


HEADERS  += \
    $$PWD/src/Widget/VideoPlayerWidget.h \
    $$PWD/src/Widget/ShowVideoWidget.h \
    $$PWD/src/Widget/VideoSlider.h

FORMS    += \
    $$PWD/src/Widget/VideoPlayerWidget.ui \
    $$PWD/src/Widget/ShowVideoWidget.ui

RESOURCES += \
    $$PWD/resources/resources.qrc

INCLUDEPATH += $$PWD/src

win32:RC_FILE=$$PWD/resources/main.rc
