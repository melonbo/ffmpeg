/****************************************************************************
** Meta object code from reading C++ file 'QPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QPlayer Base FFPlayer/QPlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QPlayer_t {
    QByteArrayData data[23];
    char stringdata0[374];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QPlayer_t qt_meta_stringdata_QPlayer = {
    {
QT_MOC_LITERAL(0, 0, 7), // "QPlayer"
QT_MOC_LITERAL(1, 8, 19), // "on_btn_play_clicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 20), // "on_btn_pause_clicked"
QT_MOC_LITERAL(4, 50, 20), // "on_btn_close_clicked"
QT_MOC_LITERAL(5, 71, 23), // "on_btn_speed_up_clicked"
QT_MOC_LITERAL(6, 95, 24), // "on_btn_open_file_clicked"
QT_MOC_LITERAL(7, 120, 15), // "slot_SDLPresent"
QT_MOC_LITERAL(8, 136, 4), // "data"
QT_MOC_LITERAL(9, 141, 15), // "slot_RGBPresent"
QT_MOC_LITERAL(10, 157, 3), // "img"
QT_MOC_LITERAL(11, 161, 22), // "on_btn_forward_clicked"
QT_MOC_LITERAL(12, 184, 23), // "on_btn_backward_clicked"
QT_MOC_LITERAL(13, 208, 24), // "on_hSlider_sliderPressed"
QT_MOC_LITERAL(14, 233, 25), // "on_hSlider_sliderReleased"
QT_MOC_LITERAL(15, 259, 16), // "slot_slider_init"
QT_MOC_LITERAL(16, 276, 8), // "duration"
QT_MOC_LITERAL(17, 285, 19), // "slot_setCurrentTime"
QT_MOC_LITERAL(18, 305, 3), // "sec"
QT_MOC_LITERAL(19, 309, 17), // "slot_setPlayValue"
QT_MOC_LITERAL(20, 327, 3), // "val"
QT_MOC_LITERAL(21, 331, 25), // "on_btn_speed_down_clicked"
QT_MOC_LITERAL(22, 357, 16) // "slot_timer_speed"

    },
    "QPlayer\0on_btn_play_clicked\0\0"
    "on_btn_pause_clicked\0on_btn_close_clicked\0"
    "on_btn_speed_up_clicked\0"
    "on_btn_open_file_clicked\0slot_SDLPresent\0"
    "data\0slot_RGBPresent\0img\0"
    "on_btn_forward_clicked\0on_btn_backward_clicked\0"
    "on_hSlider_sliderPressed\0"
    "on_hSlider_sliderReleased\0slot_slider_init\0"
    "duration\0slot_setCurrentTime\0sec\0"
    "slot_setPlayValue\0val\0on_btn_speed_down_clicked\0"
    "slot_timer_speed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    1,   99,    2, 0x08 /* Private */,
       9,    1,  102,    2, 0x08 /* Private */,
      11,    0,  105,    2, 0x08 /* Private */,
      12,    0,  106,    2, 0x08 /* Private */,
      13,    0,  107,    2, 0x08 /* Private */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    1,  109,    2, 0x08 /* Private */,
      17,    1,  112,    2, 0x08 /* Private */,
      19,    1,  115,    2, 0x08 /* Private */,
      21,    0,  118,    2, 0x08 /* Private */,
      22,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::VoidStar,    8,
    QMetaType::Void, QMetaType::QImage,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QPlayer *_t = static_cast<QPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_play_clicked(); break;
        case 1: _t->on_btn_pause_clicked(); break;
        case 2: _t->on_btn_close_clicked(); break;
        case 3: _t->on_btn_speed_up_clicked(); break;
        case 4: _t->on_btn_open_file_clicked(); break;
        case 5: _t->slot_SDLPresent((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 6: _t->slot_RGBPresent((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 7: _t->on_btn_forward_clicked(); break;
        case 8: _t->on_btn_backward_clicked(); break;
        case 9: _t->on_hSlider_sliderPressed(); break;
        case 10: _t->on_hSlider_sliderReleased(); break;
        case 11: _t->slot_slider_init((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->slot_setCurrentTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slot_setPlayValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_btn_speed_down_clicked(); break;
        case 15: _t->slot_timer_speed(); break;
        default: ;
        }
    }
}

const QMetaObject QPlayer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QPlayer.data,
      qt_meta_data_QPlayer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QPlayer.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
