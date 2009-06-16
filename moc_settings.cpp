/****************************************************************************
** Meta object code from reading C++ file 'settings.h'
**
** Created: Mon Jun 23 19:51:57 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "settings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION <= 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Settings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,    9,    9,    9, 0x0a,
      37,    9,    9,    9, 0x0a,
      52,   46,    9,    9, 0x0a,
      81,   75,    9,    9, 0x0a,
     104,    9,    9,    9, 0x0a,
     115,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Settings[] = {
    "Settings\0\0settingsChanged()\0accept()\0"
    "reject()\0index\0algorithmSelected(int)\0"
    "theme\0themeSelected(QString)\0addTheme()\0"
    "removeTheme()\0"
};

const QMetaObject Settings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Settings,
      qt_meta_data_Settings, 0 }
};

const QMetaObject *Settings::metaObject() const
{
    return &staticMetaObject;
}

void *Settings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Settings))
	return static_cast<void*>(const_cast< Settings*>(this));
    return QDialog::qt_metacast(_clname);
}

int Settings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: settingsChanged(); break;
        case 1: accept(); break;
        case 2: reject(); break;
        case 3: algorithmSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: themeSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: addTheme(); break;
        case 6: removeTheme(); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Settings::settingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
