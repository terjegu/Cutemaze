/****************************************************************************
** Meta object code from reading C++ file 'board.h'
**
** Created: Tue Jun 16 10:35:48 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "board.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'board.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Board[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,    7,    6,    6, 0x05,
      38,   34,    6,    6, 0x05,
      88,   59,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
     114,    6,    6,    6, 0x0a,
     124,    6,    6,    6, 0x0a,
     135,    6,    6,    6, 0x0a,
     153,  146,    6,    6, 0x0a,
     169,    6,    6,    6, 0x0a,
     184,    6,    6,    6, 0x08,
     199,    6,    6,    6, 0x08,
     221,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Board[] = {
    "Board\0\0checked\0pauseChecked(bool)\0run\0"
    "pauseAvailable(bool)\0seconds,steps,algorithm,size\0"
    "finished(int,int,int,int)\0newGame()\0"
    "loadGame()\0saveGame()\0paused\0"
    "pauseGame(bool)\0loadSettings()\0"
    "focusChanged()\0updateStatusMessage()\0"
    "move()\0"
};

const QMetaObject Board::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Board,
      qt_meta_data_Board, 0 }
};

const QMetaObject *Board::metaObject() const
{
    return &staticMetaObject;
}

void *Board::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Board))
        return static_cast<void*>(const_cast< Board*>(this));
    return QWidget::qt_metacast(_clname);
}

int Board::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pauseChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: pauseAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: newGame(); break;
        case 4: loadGame(); break;
        case 5: saveGame(); break;
        case 6: pauseGame((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: loadSettings(); break;
        case 8: focusChanged(); break;
        case 9: updateStatusMessage(); break;
        case 10: move(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Board::pauseChecked(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Board::pauseAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Board::finished(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
