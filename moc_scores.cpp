/****************************************************************************
** Meta object code from reading C++ file 'scores.h'
**
** Created: Mon Jun 23 19:51:57 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "scores.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scores.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION <= 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Scores[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      37,    8,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Scores[] = {
    "Scores\0\0steps,seconds,algorithm,size\0"
    "addScore(int,int,int,int)\0"
};

const QMetaObject Scores::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Scores,
      qt_meta_data_Scores, 0 }
};

const QMetaObject *Scores::metaObject() const
{
    return &staticMetaObject;
}

void *Scores::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Scores))
	return static_cast<void*>(const_cast< Scores*>(this));
    return QDialog::qt_metacast(_clname);
}

int Scores::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addScore((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        }
        _id -= 1;
    }
    return _id;
}
