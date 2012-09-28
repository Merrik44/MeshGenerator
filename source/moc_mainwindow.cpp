/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Sep 26 00:04:16 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      57,   11,   11,   11, 0x08,
      84,   11,   11,   11, 0x08,
     111,   11,   11,   11, 0x08,
     143,   12,   11,   11, 0x08,
     176,   11,   11,   11, 0x08,
     213,  205,   11,   11, 0x08,
     242,   11,   11,   11, 0x08,
     266,   11,   11,   11, 0x08,
     292,  205,   11,   11, 0x08,
     324,  205,   11,   11, 0x08,
     354,  205,   11,   11, 0x08,
     389,   12,   11,   11, 0x08,
     424,  205,   11,   11, 0x08,
     456,  205,   11,   11, 0x08,
     490,  205,   11,   11, 0x08,
     526,   11,   11,   11, 0x08,
     555,   11,   11,   11, 0x08,
     584,   12,   11,   11, 0x08,
     621,   11,   11,   11, 0x08,
     653,  205,   11,   11, 0x08,
     689,  205,   11,   11, 0x08,
     725,  205,   11,   11, 0x08,
     761,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0value\0"
    "on_subdivisionSlider_valueChanged(int)\0"
    "on_loadObjButton_clicked()\0"
    "on_loadLstButton_clicked()\0"
    "on_generateMeshButton_clicked()\0"
    "on_countSlider_valueChanged(int)\0"
    "on_exportObjButton_clicked()\0checked\0"
    "on_texCheckBox_toggled(bool)\0"
    "on_subDivPlus_clicked()\0"
    "on_subDivPlus_2_clicked()\0"
    "on_meshCheckBox_2_toggled(bool)\0"
    "on_meshCheckBox_toggled(bool)\0"
    "on_wireFrameCheckBox_toggled(bool)\0"
    "on_segmentSlider_valueChanged(int)\0"
    "on_meshCheckBox_3_toggled(bool)\0"
    "on_simplifyCheckBox_toggled(bool)\0"
    "on_simplifyCheckBox_2_toggled(bool)\0"
    "on_loadLstButton_2_clicked()\0"
    "on_loadLstButton_3_clicked()\0"
    "on_segmentSlider_2_valueChanged(int)\0"
    "on_simplifyCheckBox_3_clicked()\0"
    "on_simplifyCheckBox_3_toggled(bool)\0"
    "on_simplifyCheckBox_4_toggled(bool)\0"
    "on_simplifyCheckBox_5_toggled(bool)\0"
    "on_pushButton_clicked()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_subdivisionSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_loadObjButton_clicked(); break;
        case 2: _t->on_loadLstButton_clicked(); break;
        case 3: _t->on_generateMeshButton_clicked(); break;
        case 4: _t->on_countSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_exportObjButton_clicked(); break;
        case 6: _t->on_texCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_subDivPlus_clicked(); break;
        case 8: _t->on_subDivPlus_2_clicked(); break;
        case 9: _t->on_meshCheckBox_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_meshCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_wireFrameCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_segmentSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_meshCheckBox_3_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->on_simplifyCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->on_simplifyCheckBox_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->on_loadLstButton_2_clicked(); break;
        case 17: _t->on_loadLstButton_3_clicked(); break;
        case 18: _t->on_segmentSlider_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_simplifyCheckBox_3_clicked(); break;
        case 20: _t->on_simplifyCheckBox_3_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->on_simplifyCheckBox_4_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->on_simplifyCheckBox_5_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
