#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

class Database : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QSqlQueryModel* tableModel READ tableModel NOTIFY tableChanged)
    Q_PROPERTY(QString currentTableDisplayName READ currentTableDisplayName NOTIFY tableChanged)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY userRoleChanged)
    Q_PROPERTY(bool isBarber READ isBarber NOTIFY userRoleChanged)
    Q_PROPERTY(QString currentBarberName READ currentBarberName NOTIFY userRoleChanged)

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    Q_INVOKABLE QStringList getBarberList();
    Q_INVOKABLE bool loginAsBarber(const QString &barberName);
    Q_INVOKABLE bool adminLogin(const QString &password);
    Q_INVOKABLE void logout();

    Q_INVOKABLE void loadTable(const QString &tableName, const QString &searchText = "");
    Q_INVOKABLE QString getCurrentTable() const;
    Q_INVOKABLE QString getTableDisplayName(const QString &tableName) const;

    Q_INVOKABLE QVariantMap getRecordData(const QVariant &id);
    Q_INVOKABLE QVariantList getColumnsMetadata();
    Q_INVOKABLE QVariantList getLookupData(const QString &fieldName);
    Q_INVOKABLE QVariantList getBarbersList();
    Q_INVOKABLE QVariantList getCatalogData(const QString &category);
    Q_INVOKABLE QVariantList getAllServices();
    Q_INVOKABLE QVariantList getClientsList();

    Q_INVOKABLE QString insertRecord(const QVariantList &values);
    Q_INVOKABLE QString updateRecord(const QVariant &id, const QVariantList &values);
    Q_INVOKABLE QString deleteRecord(const QVariant &id);

    Q_INVOKABLE QString createTask(const QVariantMap &taskData);
    Q_INVOKABLE QString markTaskStarted(const QVariant &taskId);
    Q_INVOKABLE QString markTaskCompleted(const QVariant &taskId);
    Q_INVOKABLE QString markTaskCancelled(const QVariant &taskId);
    Q_INVOKABLE QString acceptTask(const QVariant &taskId);
    Q_INVOKABLE QString completeTask(const QVariant &taskId);

    Q_INVOKABLE QVariantMap getBarberInfo();
    Q_INVOKABLE QVariantList getBarberActiveTasks();
    Q_INVOKABLE QVariantList getBarberHistory();

    Q_INVOKABLE QVariantList getCashJournal();
    Q_INVOKABLE QVariantList getPreviousJournals();
    Q_INVOKABLE void startNewDay();
    Q_INVOKABLE double getCurrentDayTotal();
    Q_INVOKABLE int getTodayClientsCount();
    Q_INVOKABLE QString clearCashJournal();

    Q_INVOKABLE QString exportToPDF(const QString &fileUrl);
    Q_INVOKABLE QString exportToHTML(const QString &fileUrl);

    QSqlQueryModel *tableModel() const;
    QString currentTableDisplayName() const;
    bool isAdmin() const;
    bool isBarber() const;
    QString currentBarberName() const;

signals:
    void tableChanged();
    void userRoleChanged();
    void loginSuccess();
    void loginFailed(const QString &message);
    void logoutSuccess();

private:
    QSqlDatabase m_db;
    QSqlQueryModel *m_tableModel = nullptr;
    QString m_currentTable;
    QMap<QString, QString> m_nameMap;
    bool m_isAdmin = false;
    bool m_isBarber = false;
    QString m_currentBarberName;
    int m_currentBarberId = -1;

    void initDB();
    void createTables();
    void createAdminIfNeeded();
    QString escapeLike(const QString &value) const;
};

#endif // DATABASE_H
