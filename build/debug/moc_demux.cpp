/****************************************************************************
** Meta object code from reading C++ file 'demux.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QPlayer Base FFPlayer/demux.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'demux.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_demux_t {
    QByteArrayData data[10];
    char stringdata0[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_demux_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_demux_t qt_meta_stringdata_demux = {
    {
QT_MOC_LITERAL(0, 0, 5), // "demux"
QT_MOC_LITERAL(1, 6, 11), // "sigYUVFrame"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 4), // "data"
QT_MOC_LITERAL(4, 24, 11), // "sigRGBFrame"
QT_MOC_LITERAL(5, 36, 3), // "img"
QT_MOC_LITERAL(6, 40, 11), // "sigDuration"
QT_MOC_LITERAL(7, 52, 8), // "duration"
QT_MOC_LITERAL(8, 61, 14), // "sigCurrentTime"
QT_MOC_LITERAL(9, 76, 3) // "sec"

    },
    "demux\0sigYUVFrame\0\0data\0sigRGBFrame\0"
    "img\0sigDuration\0duration\0sigCurrentTime\0"
    "sec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_demux[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,
       8,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Int, QMetaType::VoidStar,    3,
    QMetaType::Int, QMetaType::QImage,    5,
    QMetaType::Int, QMetaType::Int,    7,
    QMetaType::Int, QMetaType::Int,    9,

       0        // eod
};

void demux::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        demux *_t = static_cast<demux *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->sigYUVFrame((*reinterpret_cast< void*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->sigRGBFrame((*reinterpret_cast< QImage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->sigDuration((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->sigCurrentTime((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef int (demux::*_t)(void * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&demux::sigYUVFrame)) {
                *result = 0;
                return;
            }
        }
        {
            typedef int (demux::*_t)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&demux::sigRGBFrame)) {
                *result = 1;
                return;
            }
        }
        {
            typedef int (demux::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&demux::sigDuration)) {
                *result = 2;
                return;
            }
        }
        {
            typedef int (demux::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&demux::sigCurrentTime)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject demux::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_demux.data,
      qt_meta_data_demux,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *demux::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *demux::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_demux.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int demux::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
int demux::sigYUVFrame(void * _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}

// SIGNAL 1
int demux::sigRGBFrame(QImage _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
    return _t0;
}

// SIGNAL 2
int demux::sigDuration(int _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
    return _t0;
}

// SIGNAL 3
int demux::sigCurrentTime(int _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
