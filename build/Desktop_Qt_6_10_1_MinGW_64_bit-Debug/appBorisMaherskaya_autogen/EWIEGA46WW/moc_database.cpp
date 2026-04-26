/****************************************************************************
** Meta object code from reading C++ file 'database.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../database.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'database.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN8DatabaseE_t {};
} // unnamed namespace

template <> constexpr inline auto Database::qt_create_metaobjectdata<qt_meta_tag_ZN8DatabaseE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Database",
        "QML.Element",
        "auto",
        "QML.Singleton",
        "true",
        "tableChanged",
        "",
        "userRoleChanged",
        "loginSuccess",
        "loginFailed",
        "message",
        "logoutSuccess",
        "getBarberList",
        "loginAsBarber",
        "barberName",
        "adminLogin",
        "password",
        "logout",
        "loadTable",
        "tableName",
        "searchText",
        "getCurrentTable",
        "getTableDisplayName",
        "getRecordData",
        "QVariantMap",
        "QVariant",
        "id",
        "getColumnsMetadata",
        "QVariantList",
        "getLookupData",
        "fieldName",
        "getBarbersList",
        "getCatalogData",
        "category",
        "getAllServices",
        "getClientsList",
        "insertRecord",
        "values",
        "updateRecord",
        "deleteRecord",
        "createTask",
        "taskData",
        "markTaskStarted",
        "taskId",
        "markTaskCompleted",
        "markTaskCancelled",
        "acceptTask",
        "completeTask",
        "getBarberInfo",
        "getBarberActiveTasks",
        "getBarberHistory",
        "getCashJournal",
        "getPreviousJournals",
        "startNewDay",
        "getCurrentDayTotal",
        "getTodayClientsCount",
        "clearCashJournal",
        "exportToPDF",
        "fileUrl",
        "exportToHTML",
        "tableModel",
        "QSqlQueryModel*",
        "currentTableDisplayName",
        "isAdmin",
        "isBarber",
        "currentBarberName"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'tableChanged'
        QtMocHelpers::SignalData<void()>(5, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'userRoleChanged'
        QtMocHelpers::SignalData<void()>(7, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loginSuccess'
        QtMocHelpers::SignalData<void()>(8, 6, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loginFailed'
        QtMocHelpers::SignalData<void(const QString &)>(9, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'logoutSuccess'
        QtMocHelpers::SignalData<void()>(11, 6, QMC::AccessPublic, QMetaType::Void),
        // Method 'getBarberList'
        QtMocHelpers::MethodData<QStringList()>(12, 6, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'loginAsBarber'
        QtMocHelpers::MethodData<bool(const QString &)>(13, 6, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 14 },
        }}),
        // Method 'adminLogin'
        QtMocHelpers::MethodData<bool(const QString &)>(15, 6, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 16 },
        }}),
        // Method 'logout'
        QtMocHelpers::MethodData<void()>(17, 6, QMC::AccessPublic, QMetaType::Void),
        // Method 'loadTable'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(18, 6, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 }, { QMetaType::QString, 20 },
        }}),
        // Method 'loadTable'
        QtMocHelpers::MethodData<void(const QString &)>(18, 6, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Method 'getCurrentTable'
        QtMocHelpers::MethodData<QString() const>(21, 6, QMC::AccessPublic, QMetaType::QString),
        // Method 'getTableDisplayName'
        QtMocHelpers::MethodData<QString(const QString &) const>(22, 6, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 19 },
        }}),
        // Method 'getRecordData'
        QtMocHelpers::MethodData<QVariantMap(const QVariant &)>(23, 6, QMC::AccessPublic, 0x80000000 | 24, {{
            { 0x80000000 | 25, 26 },
        }}),
        // Method 'getColumnsMetadata'
        QtMocHelpers::MethodData<QVariantList()>(27, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'getLookupData'
        QtMocHelpers::MethodData<QVariantList(const QString &)>(29, 6, QMC::AccessPublic, 0x80000000 | 28, {{
            { QMetaType::QString, 30 },
        }}),
        // Method 'getBarbersList'
        QtMocHelpers::MethodData<QVariantList()>(31, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'getCatalogData'
        QtMocHelpers::MethodData<QVariantList(const QString &)>(32, 6, QMC::AccessPublic, 0x80000000 | 28, {{
            { QMetaType::QString, 33 },
        }}),
        // Method 'getAllServices'
        QtMocHelpers::MethodData<QVariantList()>(34, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'getClientsList'
        QtMocHelpers::MethodData<QVariantList()>(35, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'insertRecord'
        QtMocHelpers::MethodData<QString(const QVariantList &)>(36, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 28, 37 },
        }}),
        // Method 'updateRecord'
        QtMocHelpers::MethodData<QString(const QVariant &, const QVariantList &)>(38, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 26 }, { 0x80000000 | 28, 37 },
        }}),
        // Method 'deleteRecord'
        QtMocHelpers::MethodData<QString(const QVariant &)>(39, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 26 },
        }}),
        // Method 'createTask'
        QtMocHelpers::MethodData<QString(const QVariantMap &)>(40, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 24, 41 },
        }}),
        // Method 'markTaskStarted'
        QtMocHelpers::MethodData<QString(const QVariant &)>(42, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 43 },
        }}),
        // Method 'markTaskCompleted'
        QtMocHelpers::MethodData<QString(const QVariant &)>(44, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 43 },
        }}),
        // Method 'markTaskCancelled'
        QtMocHelpers::MethodData<QString(const QVariant &)>(45, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 43 },
        }}),
        // Method 'acceptTask'
        QtMocHelpers::MethodData<QString(const QVariant &)>(46, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 43 },
        }}),
        // Method 'completeTask'
        QtMocHelpers::MethodData<QString(const QVariant &)>(47, 6, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 25, 43 },
        }}),
        // Method 'getBarberInfo'
        QtMocHelpers::MethodData<QVariantMap()>(48, 6, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'getBarberActiveTasks'
        QtMocHelpers::MethodData<QVariantList()>(49, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'getBarberHistory'
        QtMocHelpers::MethodData<QVariantList()>(50, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'getCashJournal'
        QtMocHelpers::MethodData<QVariantList()>(51, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'getPreviousJournals'
        QtMocHelpers::MethodData<QVariantList()>(52, 6, QMC::AccessPublic, 0x80000000 | 28),
        // Method 'startNewDay'
        QtMocHelpers::MethodData<void()>(53, 6, QMC::AccessPublic, QMetaType::Void),
        // Method 'getCurrentDayTotal'
        QtMocHelpers::MethodData<double()>(54, 6, QMC::AccessPublic, QMetaType::Double),
        // Method 'getTodayClientsCount'
        QtMocHelpers::MethodData<int()>(55, 6, QMC::AccessPublic, QMetaType::Int),
        // Method 'clearCashJournal'
        QtMocHelpers::MethodData<QString()>(56, 6, QMC::AccessPublic, QMetaType::QString),
        // Method 'exportToPDF'
        QtMocHelpers::MethodData<QString(const QString &)>(57, 6, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 58 },
        }}),
        // Method 'exportToHTML'
        QtMocHelpers::MethodData<QString(const QString &)>(59, 6, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 58 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'tableModel'
        QtMocHelpers::PropertyData<QSqlQueryModel*>(60, 0x80000000 | 61, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'currentTableDisplayName'
        QtMocHelpers::PropertyData<QString>(62, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'isAdmin'
        QtMocHelpers::PropertyData<bool>(63, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'isBarber'
        QtMocHelpers::PropertyData<bool>(64, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'currentBarberName'
        QtMocHelpers::PropertyData<QString>(65, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
    });
    return QtMocHelpers::metaObjectData<Database, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject Database::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8DatabaseE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8DatabaseE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8DatabaseE_t>.metaTypes,
    nullptr
} };

void Database::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Database *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->tableChanged(); break;
        case 1: _t->userRoleChanged(); break;
        case 2: _t->loginSuccess(); break;
        case 3: _t->loginFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->logoutSuccess(); break;
        case 5: { QStringList _r = _t->getBarberList();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->loginAsBarber((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->adminLogin((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->logout(); break;
        case 9: _t->loadTable((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->loadTable((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: { QString _r = _t->getCurrentTable();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 12: { QString _r = _t->getTableDisplayName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantMap _r = _t->getRecordData((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 14: { QVariantList _r = _t->getColumnsMetadata();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 15: { QVariantList _r = _t->getLookupData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 16: { QVariantList _r = _t->getBarbersList();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 17: { QVariantList _r = _t->getCatalogData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 18: { QVariantList _r = _t->getAllServices();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 19: { QVariantList _r = _t->getClientsList();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 20: { QString _r = _t->insertRecord((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 21: { QString _r = _t->updateRecord((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 22: { QString _r = _t->deleteRecord((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 23: { QString _r = _t->createTask((*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 24: { QString _r = _t->markTaskStarted((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 25: { QString _r = _t->markTaskCompleted((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 26: { QString _r = _t->markTaskCancelled((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 27: { QString _r = _t->acceptTask((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 28: { QString _r = _t->completeTask((*reinterpret_cast<std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 29: { QVariantMap _r = _t->getBarberInfo();
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 30: { QVariantList _r = _t->getBarberActiveTasks();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 31: { QVariantList _r = _t->getBarberHistory();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 32: { QVariantList _r = _t->getCashJournal();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 33: { QVariantList _r = _t->getPreviousJournals();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 34: _t->startNewDay(); break;
        case 35: { double _r = _t->getCurrentDayTotal();
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 36: { int _r = _t->getTodayClientsCount();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 37: { QString _r = _t->clearCashJournal();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 38: { QString _r = _t->exportToPDF((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 39: { QString _r = _t->exportToHTML((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Database::*)()>(_a, &Database::tableChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Database::*)()>(_a, &Database::userRoleChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Database::*)()>(_a, &Database::loginSuccess, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Database::*)(const QString & )>(_a, &Database::loginFailed, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Database::*)()>(_a, &Database::logoutSuccess, 4))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSqlQueryModel* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QSqlQueryModel**>(_v) = _t->tableModel(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->currentTableDisplayName(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isAdmin(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->isBarber(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->currentBarberName(); break;
        default: break;
        }
    }
}

const QMetaObject *Database::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Database::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8DatabaseE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Database::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 40;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Database::tableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Database::userRoleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Database::loginSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Database::loginFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Database::logoutSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
