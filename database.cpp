#include "database.h"

#include <QCoreApplication>
#include <QFile>
#include <QPdfWriter>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextDocument>
#include <QUrl>
#include <QDateTime>
#include <QDebug>
#include <QtMath>

Database::Database(QObject *parent)
    : QObject(parent)
{
    m_tableModel = new QSqlQueryModel(this);

    m_nameMap["Haircuts"] = "Прически";
    m_nameMap["Services"] = "Услуги";
    m_nameMap["Barbers"] = "Сотрудники";
    m_nameMap["Products"] = "Товары";
    m_nameMap["Tasks"] = "Запросы на услуги";
    m_nameMap["Clients"] = "Клиенты";
    m_nameMap["CashJournal"] = "Кассовый журнал";

    initDB();
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

QSqlQueryModel *Database::tableModel() const { return m_tableModel; }

QString Database::currentTableDisplayName() const
{
    return m_nameMap.value(m_currentTable, m_currentTable);
}

bool Database::isAdmin() const { return m_isAdmin; }
bool Database::isBarber() const { return m_isBarber; }
QString Database::currentBarberName() const { return m_currentBarberName; }
QString Database::getCurrentTable() const { return m_currentTable; }

QString Database::getTableDisplayName(const QString &tableName) const
{
    return m_nameMap.value(tableName, tableName);
}

QString Database::escapeLike(const QString &value) const
{
    QString safe = value;
    safe.replace("'", "''");
    return safe;
}

void Database::initDB()
{
    if (QSqlDatabase::contains("barber_connection")) {
        m_db = QSqlDatabase::database("barber_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "barber_connection");
    }

    const QString appPath = QCoreApplication::applicationDirPath();
    m_db.setDatabaseName(appPath + "/barbershop.db");

    if (!m_db.open()) {
        qWarning() << "Failed to open DB:" << m_db.lastError().text();
        return;
    }

    QSqlQuery pragma(m_db);
    pragma.exec("PRAGMA foreign_keys = ON;");
    pragma.exec("PRAGMA journal_mode=WAL;");

    createTables();
    createAdminIfNeeded();
}

void Database::createAdminIfNeeded()
{
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM Admins");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO Admins(login, password) VALUES('admin', 'admin123')");
    }
}

void Database::createTables()
{
    QSqlQuery query(m_db);

    query.exec("CREATE TABLE IF NOT EXISTS Admins (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT NOT NULL UNIQUE, password TEXT NOT NULL)");
    query.exec("CREATE TABLE IF NOT EXISTS Haircuts (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, description TEXT, price REAL NOT NULL, duration_minutes INTEGER NOT NULL)");
    query.exec("CREATE TABLE IF NOT EXISTS Services (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, description TEXT, price REAL NOT NULL, duration_minutes INTEGER NOT NULL)");
    query.exec("CREATE TABLE IF NOT EXISTS Barbers (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE, experience INTEGER NOT NULL DEFAULT 0, specializations TEXT, services_completed INTEGER NOT NULL DEFAULT 0, commission_rate REAL NOT NULL DEFAULT 30.0, total_earned REAL NOT NULL DEFAULT 0.0)");
    query.exec("CREATE TABLE IF NOT EXISTS Products (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, description TEXT, price REAL NOT NULL)");
    query.exec("CREATE TABLE IF NOT EXISTS Clients (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE, services_count INTEGER NOT NULL DEFAULT 0)");
    query.exec("CREATE TABLE IF NOT EXISTS Tasks (id INTEGER PRIMARY KEY AUTOINCREMENT, client_name TEXT, client_id INTEGER REFERENCES Clients(id), haircut_id INTEGER REFERENCES Haircuts(id), service_ids TEXT, product_ids TEXT, barber_ids TEXT, scheduled_time TEXT, total_price REAL NOT NULL DEFAULT 0.0, status TEXT NOT NULL DEFAULT 'pending' CHECK(status IN ('pending','active','completed','cancelled')), created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, completed_at TEXT, assigned_barber_id INTEGER REFERENCES Barbers(id), barber_earned REAL DEFAULT 0.0)");
    query.exec("CREATE TABLE IF NOT EXISTS CashJournal (id INTEGER PRIMARY KEY AUTOINCREMENT, type TEXT NOT NULL, description TEXT, amount REAL NOT NULL, barber_name TEXT, client_name TEXT, created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, date TEXT NOT NULL)");

    QSqlQuery checkQuery(m_db);
    checkQuery.exec("PRAGMA table_info(Tasks)");
    bool hasClientId = false;
    while (checkQuery.next()) {
        if (checkQuery.value(1).toString() == "client_id") {
            hasClientId = true;
            break;
        }
    }
    if (!hasClientId) {
        query.exec("ALTER TABLE Tasks ADD COLUMN client_id INTEGER REFERENCES Clients(id)");
    }
}

QStringList Database::getBarberList()
{
    QStringList list;
    QSqlQuery query(m_db);
    query.exec("SELECT name FROM Barbers ORDER BY name");
    while (query.next()) list << query.value(0).toString();
    return list;
}

bool Database::loginAsBarber(const QString &barberName)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name FROM Barbers WHERE name = ?");
    query.addBindValue(barberName);
    if (query.exec() && query.next()) {
        m_currentBarberId = query.value(0).toInt();
        m_currentBarberName = query.value(1).toString();
        m_isBarber = true;
        m_isAdmin = false;
        emit userRoleChanged();
        emit loginSuccess();
        return true;
    }
    emit loginFailed("Парикмахер не найден");
    return false;
}

bool Database::adminLogin(const QString &password)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Admins WHERE password = ?");
    query.addBindValue(password);
    if (query.exec() && query.next()) {
        m_isAdmin = true;
        m_isBarber = false;
        m_currentBarberId = -1;
        m_currentBarberName.clear();
        emit userRoleChanged();
        emit loginSuccess();
        return true;
    }
    emit loginFailed("Неверный пароль");
    return false;
}

void Database::logout()
{
    m_isAdmin = false;
    m_isBarber = false;
    m_currentBarberId = -1;
    m_currentBarberName.clear();
    m_currentTable.clear();
    emit userRoleChanged();
    emit logoutSuccess();
}

void Database::loadTable(const QString &tableName, const QString &searchText)
{
    m_currentTable = tableName;
    const QString safe = escapeLike(searchText.trimmed());
    QString queryStr;

    if (tableName == "Haircuts") {
        queryStr = "SELECT id, name AS 'Название', description AS 'Описание', price AS 'Цена', duration_minutes AS 'Длительность (мин)' FROM Haircuts";
        if (!safe.isEmpty()) queryStr += " WHERE name LIKE '%" + safe + "%'";
        queryStr += " ORDER BY name";
    } else if (tableName == "Services") {
        queryStr = "SELECT id, name AS 'Название', description AS 'Описание', price AS 'Цена', duration_minutes AS 'Длительность (мин)' FROM Services";
        if (!safe.isEmpty()) queryStr += " WHERE name LIKE '%" + safe + "%'";
        queryStr += " ORDER BY name";
    } else if (tableName == "Barbers") {
        queryStr = "SELECT id, name AS 'Имя', experience AS 'Стаж (лет)', specializations AS 'Специализации', services_completed AS 'Выполнено услуг', commission_rate AS 'Ставка (%)', total_earned AS 'Заработано' FROM Barbers";
        if (!safe.isEmpty()) queryStr += " WHERE name LIKE '%" + safe + "%'";
        queryStr += " ORDER BY name";
    } else if (tableName == "Products") {
        queryStr = "SELECT id, name AS 'Название', description AS 'Описание', price AS 'Цена' FROM Products";
        if (!safe.isEmpty()) queryStr += " WHERE name LIKE '%" + safe + "%'";
        queryStr += " ORDER BY name";
    } else if (tableName == "Clients") {
        queryStr = "SELECT id, name AS 'ФИО', services_count AS 'Кол-во услуг' FROM Clients";
        if (!safe.isEmpty()) queryStr += " WHERE name LIKE '%" + safe + "%'";
        queryStr += " ORDER BY name";
    } else if (tableName == "Tasks") {
        queryStr = "SELECT t.id, t.client_name AS 'Клиент', COALESCE(h.name,'-') AS 'Прическа', t.scheduled_time AS 'Назначено на', t.total_price AS 'Сумма', CASE t.status WHEN 'pending' THEN 'Ожидает' WHEN 'active' THEN 'В работе' WHEN 'completed' THEN 'Выполнен' WHEN 'cancelled' THEN 'Отказ' END AS 'Статус', COALESCE(b.name,'-') AS 'Исполнитель' FROM Tasks t LEFT JOIN Haircuts h ON h.id=t.haircut_id LEFT JOIN Barbers b ON b.id=t.assigned_barber_id";
        if (!safe.isEmpty()) queryStr += " WHERE t.client_name LIKE '%" + safe + "%'";
        queryStr += " ORDER BY t.created_at DESC";
    } else if (tableName == "CashJournal") {
        queryStr = "SELECT id, type AS 'Тип', description AS 'Описание', amount AS 'Сумма', barber_name AS 'Мастер', client_name AS 'Клиент', created_at AS 'Время' FROM CashJournal ORDER BY created_at DESC";
    }

    m_tableModel->setQuery(queryStr, m_db);
    emit tableChanged();
}

QVariantList Database::getColumnsMetadata()
{
    QVariantList columns;
    QSqlRecord rec = m_db.record(m_currentTable);

    for (int i = 0; i < rec.count(); ++i) {
        QString field = rec.fieldName(i);
        QVariantMap col;
        col["name"] = field;
        col["display"] = field;
        col["required"] = false;
        col["isLookup"] = false;
        col["isHidden"] = false;
        col["isSpecialization"] = false;

        if (field == "id") col["isHidden"] = true;
        if (m_currentTable == "Barbers" && (field == "services_completed" || field == "total_earned")) col["isHidden"] = true;
        if (m_currentTable == "Clients" && field == "services_count") col["isHidden"] = true;

        if (m_currentTable == "Haircuts" || m_currentTable == "Services") {
            if (field == "name") { col["display"] = "Название"; col["required"] = true; }
            else if (field == "description") col["display"] = "Описание";
            else if (field == "price") { col["display"] = "Цена"; col["required"] = true; }
            else if (field == "duration_minutes") { col["display"] = "Длительность (мин)"; col["required"] = true; }
        } else if (m_currentTable == "Barbers") {
            if (field == "name") { col["display"] = "Имя"; col["required"] = true; }
            else if (field == "experience") { col["display"] = "Стаж (лет)"; }
            else if (field == "specializations") { col["display"] = "Специализации"; col["required"] = true; col["isSpecialization"] = true; }
            else if (field == "commission_rate") { col["display"] = "Ставка (%)"; col["required"] = true; }
        } else if (m_currentTable == "Products") {
            if (field == "name") { col["display"] = "Название"; col["required"] = true; }
            else if (field == "description") col["display"] = "Описание";
            else if (field == "price") { col["display"] = "Цена"; col["required"] = true; }
        } else if (m_currentTable == "Clients") {
            if (field == "name") { col["display"] = "ФИО"; col["required"] = true; }
            if (field == "services_count") col["display"] = "Кол-во услуг";
        }

        columns.append(col);
    }
    return columns;
}

QVariantList Database::getAllServices()
{
    QVariantList list;
    QSqlQuery query(m_db);

    query.exec("SELECT id, name, 'Прическа' as type FROM Haircuts ORDER BY name");
    while (query.next()) {
        QVariantMap m;
        m["id"] = "h_" + query.value(0).toString();
        m["name"] = query.value(1).toString();
        m["type"] = "прическа";
        m["displayName"] = query.value(1).toString() + " (прическа)";
        list.append(m);
    }

    query.exec("SELECT id, name, 'Услуга' as type FROM Services ORDER BY name");
    while (query.next()) {
        QVariantMap m;
        m["id"] = "s_" + query.value(0).toString();
        m["name"] = query.value(1).toString();
        m["type"] = "услуга";
        m["displayName"] = query.value(1).toString() + " (услуга)";
        list.append(m);
    }

    return list;
}

QVariantList Database::getLookupData(const QString &fieldName)
{
    QVariantList list;
    if (fieldName == "specializations") {
        return getAllServices();
    }
    return list;
}

QVariantList Database::getBarbersList()
{
    QVariantList list;
    QSqlQuery query(m_db);
    query.exec("SELECT id, name FROM Barbers ORDER BY name");
    while (query.next()) {
        QVariantMap m; m["id"] = query.value(0); m["name"] = query.value(1);
        list.append(m);
    }
    return list;
}

QVariantList Database::getClientsList()
{
    QVariantList list;
    QSqlQuery query(m_db);
    query.exec("SELECT id, name FROM Clients ORDER BY name");
    while (query.next()) {
        QVariantMap m; m["id"] = query.value(0); m["name"] = query.value(1);
        list.append(m);
    }
    return list;
}

QVariantList Database::getCatalogData(const QString &category)
{
    QVariantList list;
    QString table = (category == "haircuts") ? "Haircuts" : (category == "services") ? "Services" : "Products";
    QSqlQuery query(m_db);
    query.exec("SELECT id, name, price FROM " + table + " ORDER BY name");
    while (query.next()) {
        QVariantMap m; m["id"] = query.value(0); m["name"] = query.value(1); m["price"] = query.value(2);
        list.append(m);
    }
    return list;
}

QVariantMap Database::getRecordData(const QVariant &id)
{
    QVariantMap result;
    if (!id.isValid()) return result;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM " + m_currentTable + " WHERE id = ?");
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        QSqlRecord rec = query.record();
        for (int i = 0; i < rec.count(); ++i)
            result[rec.fieldName(i)] = query.value(i);
    }
    return result;
}

QString Database::insertRecord(const QVariantList &values)
{
    QSqlRecord rec = m_db.record(m_currentTable);
    if (rec.isEmpty()) return "Ошибка: таблица не найдена";

    QStringList cols, placeholders;
    for (int i = 0; i < rec.count(); ++i) {
        QString field = rec.fieldName(i);
        if (field == "id") continue;
        if (m_currentTable == "Barbers" && (field == "services_completed" || field == "total_earned")) continue;
        if (m_currentTable == "Clients" && field == "services_count") continue;
        cols << field;
        placeholders << "?";
    }

    if (values.size() != cols.size())
        return "Ошибка: неверное количество значений";

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO " + m_currentTable + " (" + cols.join(", ") + ") VALUES (" + placeholders.join(", ") + ")");
    for (const QVariant &v : values) query.addBindValue(v);

    if (!query.exec()) {
        QString err = query.lastError().text();
        if (err.contains("UNIQUE", Qt::CaseInsensitive)) return "Ошибка: такая запись уже существует";
        return "Ошибка: " + err;
    }

    loadTable(m_currentTable);
    return "Запись успешно добавлена";
}

QString Database::updateRecord(const QVariant &id, const QVariantList &values)
{
    if (!id.isValid()) return "Ошибка ID";
    QSqlRecord rec = m_db.record(m_currentTable);
    if (rec.isEmpty()) return "Ошибка: таблица не найдена";

    QStringList setParts;
    for (int i = 0; i < rec.count(); ++i) {
        QString field = rec.fieldName(i);
        if (field == "id") continue;
        if (m_currentTable == "Barbers" && (field == "services_completed" || field == "total_earned")) continue;
        if (m_currentTable == "Clients" && field == "services_count") continue;
        setParts << (field + " = ?");
    }

    if (values.size() != setParts.size()) return "Ошибка: неверное количество значений";

    QSqlQuery query(m_db);
    query.prepare("UPDATE " + m_currentTable + " SET " + setParts.join(", ") + " WHERE id = ?");
    for (const QVariant &v : values) query.addBindValue(v);
    query.addBindValue(id);

    if (!query.exec()) return "Ошибка: " + query.lastError().text();

    loadTable(m_currentTable);
    return "Запись успешно обновлена";
}

QString Database::deleteRecord(const QVariant &id)
{
    if (!id.isValid()) return "Ошибка ID";
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM " + m_currentTable + " WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        QString err = query.lastError().text();
        if (err.contains("FOREIGN KEY", Qt::CaseInsensitive)) return "Невозможно удалить: есть связанные записи";
        return "Ошибка: " + err;
    }
    loadTable(m_currentTable);
    return "Запись удалена";
}

QString Database::createTask(const QVariantMap &taskData)
{
    QString clientName = taskData.value("clientName").toString();
    int clientId = taskData.value("clientId").toInt();
    QVariant haircutId = taskData.value("haircutId");
    QString serviceIds = taskData.value("serviceIds").toString();
    QString productIds = taskData.value("productIds").toString();
    QString barberIds = taskData.value("barberIds").toString();
    QString scheduledTime = taskData.value("scheduledTime").toString();

    if (clientName.isEmpty()) return "Ошибка: укажите клиента";

    double totalPrice = 0;
    QSqlQuery q(m_db);

    if (haircutId.isValid() && !haircutId.toString().isEmpty()) {
        q.prepare("SELECT price FROM Haircuts WHERE id=?"); q.addBindValue(haircutId);
        if (q.exec() && q.next()) totalPrice += q.value(0).toDouble();
    }

    const QStringList sIds = serviceIds.split(",", Qt::SkipEmptyParts);
    for (const QString &sid : sIds) {
        q.prepare("SELECT price FROM Services WHERE id=?"); q.addBindValue(sid.trimmed().toInt());
        if (q.exec() && q.next()) totalPrice += q.value(0).toDouble();
    }

    const QStringList pIds = productIds.split(",", Qt::SkipEmptyParts);
    for (const QString &pid : pIds) {
        q.prepare("SELECT price FROM Products WHERE id=?"); q.addBindValue(pid.trimmed().toInt());
        if (q.exec() && q.next()) totalPrice += q.value(0).toDouble();
    }

    q.prepare("INSERT INTO Tasks(client_name, client_id, haircut_id, service_ids, product_ids, barber_ids, scheduled_time, total_price) VALUES(?,?,?,?,?,?,?,?)");
    q.addBindValue(clientName);
    q.addBindValue(clientId > 0 ? clientId : QVariant());
    q.addBindValue(haircutId);
    q.addBindValue(serviceIds);
    q.addBindValue(productIds);
    q.addBindValue(barberIds);
    q.addBindValue(scheduledTime);
    q.addBindValue(totalPrice);

    if (!q.exec()) {
        qDebug() << "Create task error:" << q.lastError().text();
        return "Ошибка: " + q.lastError().text();
    }

    loadTable("Tasks");
    return "Запрос создан. Сумма: " + QString::number(totalPrice) + " р";
}

QString Database::acceptTask(const QVariant &taskId)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE Tasks SET status='active', assigned_barber_id=? WHERE id=? AND status='pending'");
    q.addBindValue(m_currentBarberId); q.addBindValue(taskId);
    if (!q.exec()) return "Ошибка";
    if (q.numRowsAffected() == 0) return "Уже принят";
    return "Вы приняли заказ";
}

QString Database::markTaskStarted(const QVariant &taskId)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE Tasks SET status='active' WHERE id=? AND status='pending'");
    q.addBindValue(taskId);
    if (!q.exec()) return "Ошибка";
    return "В работе";
}

QString Database::completeTask(const QVariant &taskId)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT t.id, t.total_price, t.assigned_barber_id, t.client_name, t.client_id, COALESCE(h.name,''), t.scheduled_time FROM Tasks t LEFT JOIN Haircuts h ON h.id=t.haircut_id WHERE t.id=? AND t.status='active'");
    q.addBindValue(taskId);
    if (!q.exec() || !q.next()) return "Ошибка выполнения";

    int taskIdVal = q.value(0).toInt();
    double total = q.value(1).toDouble();
    int barberId = q.value(2).toInt();
    QString clientName = q.value(3).toString();
    int clientId = q.value(4).toInt();
    QString haircutName = q.value(5).toString();
    QString scheduledTime = q.value(6).toString();

    if (barberId != m_currentBarberId) return "Вы не исполнитель";

    QString barberName;
    double barberEarned = 0;
    if (barberId > 0) {
        QSqlQuery bq(m_db);
        bq.prepare("SELECT name, commission_rate FROM Barbers WHERE id=?"); bq.addBindValue(barberId);
        if (bq.exec() && bq.next()) {
            barberName = bq.value(0).toString();
            barberEarned = total * (bq.value(1).toDouble() / 100.0);
            barberEarned = qRound(barberEarned * 100.0) / 100.0;
        }
        QSqlQuery upd(m_db);
        upd.prepare("UPDATE Barbers SET services_completed=services_completed+1, total_earned=total_earned+? WHERE id=?");
        upd.addBindValue(barberEarned); upd.addBindValue(barberId);
        upd.exec();
    }

    if (clientId > 0) {
        QSqlQuery clientUpd(m_db);
        clientUpd.prepare("UPDATE Clients SET services_count = services_count + 1 WHERE id = ?");
        clientUpd.addBindValue(clientId);
        clientUpd.exec();
    }

    q.prepare("UPDATE Tasks SET status='completed', completed_at=CURRENT_TIMESTAMP, barber_earned=? WHERE id=?");
    q.addBindValue(barberEarned); q.addBindValue(taskIdVal);
    if (!q.exec()) return "Ошибка завершения";

    QString today = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QSqlQuery cj(m_db);
    cj.prepare("INSERT INTO CashJournal(type, description, amount, barber_name, client_name, date, created_at) VALUES(?,?,?,?,?,?,?)");
    cj.addBindValue("Услуга");
    cj.addBindValue(haircutName.isEmpty() ? "Стрижка" : haircutName);
    cj.addBindValue(total);
    cj.addBindValue(barberName);
    cj.addBindValue(clientName);
    cj.addBindValue(today);
    cj.addBindValue(today + " " + scheduledTime);
    cj.exec();

    loadTable("Tasks");
    return "Выполнено";
}

QString Database::markTaskCompleted(const QVariant &taskId)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT t.id, t.total_price, t.assigned_barber_id, t.client_name, t.client_id, COALESCE(h.name,''), t.scheduled_time FROM Tasks t LEFT JOIN Haircuts h ON h.id=t.haircut_id WHERE t.id=? AND t.status IN ('pending','active')");
    q.addBindValue(taskId);
    if (!q.exec() || !q.next()) return "Ошибка выполнения";

    int taskIdVal = q.value(0).toInt();
    double total = q.value(1).toDouble();
    int barberId = q.value(2).toInt();
    QString clientName = q.value(3).toString();
    int clientId = q.value(4).toInt();
    QString haircutName = q.value(5).toString();
    QString scheduledTime = q.value(6).toString();

    QString barberName;
    double barberEarned = 0;
    if (barberId > 0) {
        QSqlQuery bq(m_db);
        bq.prepare("SELECT name, commission_rate FROM Barbers WHERE id=?"); bq.addBindValue(barberId);
        if (bq.exec() && bq.next()) {
            barberName = bq.value(0).toString();
            barberEarned = total * (bq.value(1).toDouble() / 100.0);
            barberEarned = qRound(barberEarned * 100.0) / 100.0;
        }
        QSqlQuery upd(m_db);
        upd.prepare("UPDATE Barbers SET services_completed=services_completed+1, total_earned=total_earned+? WHERE id=?");
        upd.addBindValue(barberEarned); upd.addBindValue(barberId);
        upd.exec();
    } else {
        QSqlQuery infoQuery(m_db);
        infoQuery.prepare("SELECT barber_ids FROM Tasks WHERE id=?");
        infoQuery.addBindValue(taskIdVal);
        if (infoQuery.exec() && infoQuery.next()) {
            QString ids = infoQuery.value(0).toString();
            QStringList idList = ids.split(",", Qt::SkipEmptyParts);
            if (!idList.isEmpty()) {
                int firstBarberId = idList.first().trimmed().toInt();
                QSqlQuery bq(m_db);
                bq.prepare("SELECT name, commission_rate FROM Barbers WHERE id=?"); bq.addBindValue(firstBarberId);
                if (bq.exec() && bq.next()) {
                    barberName = bq.value(0).toString();
                    barberEarned = total * (bq.value(1).toDouble() / 100.0);
                    barberEarned = qRound(barberEarned * 100.0) / 100.0;
                }
                QSqlQuery upd(m_db);
                upd.prepare("UPDATE Barbers SET services_completed=services_completed+1, total_earned=total_earned+? WHERE id=?");
                upd.addBindValue(barberEarned); upd.addBindValue(firstBarberId);
                upd.exec();
            }
        }
    }

    if (clientId > 0) {
        QSqlQuery clientUpd(m_db);
        clientUpd.prepare("UPDATE Clients SET services_count = services_count + 1 WHERE id = ?");
        clientUpd.addBindValue(clientId);
        clientUpd.exec();
    }

    q.prepare("UPDATE Tasks SET status='completed', completed_at=CURRENT_TIMESTAMP, barber_earned=? WHERE id=?");
    q.addBindValue(barberEarned); q.addBindValue(taskIdVal);
    if (!q.exec()) return "Ошибка завершения";

    QString today = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QSqlQuery cj(m_db);
    cj.prepare("INSERT INTO CashJournal(type, description, amount, barber_name, client_name, date, created_at) VALUES(?,?,?,?,?,?,?)");
    cj.addBindValue("Услуга");
    cj.addBindValue(haircutName.isEmpty() ? "Стрижка" : haircutName);
    cj.addBindValue(total);
    cj.addBindValue(barberName);
    cj.addBindValue(clientName);
    cj.addBindValue(today);
    cj.addBindValue(today + " " + scheduledTime);
    cj.exec();

    loadTable("Tasks");
    return "Выполнено";
}

QString Database::markTaskCancelled(const QVariant &taskId)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE Tasks SET status='cancelled' WHERE id=? AND status != 'completed'");
    q.addBindValue(taskId);
    q.exec();
    loadTable("Tasks");
    return "Отменено";
}

QVariantMap Database::getBarberInfo()
{
    QVariantMap info;
    QSqlQuery q(m_db);
    q.prepare("SELECT * FROM Barbers WHERE id=?"); q.addBindValue(m_currentBarberId);
    if (q.exec() && q.next()) {
        info["name"] = q.value("name");
        info["experience"] = q.value("experience");
        info["specializations"] = q.value("specializations");
        info["services_completed"] = q.value("services_completed");
        info["commission_rate"] = q.value("commission_rate");
        info["total_earned"] = q.value("total_earned");
    }
    return info;
}

QVariantList Database::getBarberActiveTasks()
{
    QVariantList list;
    QSqlQuery q(m_db);
    q.prepare("SELECT t.id, t.client_name, COALESCE(h.name,'-') AS haircut, t.scheduled_time, t.total_price, t.status FROM Tasks t LEFT JOIN Haircuts h ON h.id=t.haircut_id WHERE (t.status='pending' AND t.barber_ids LIKE ?) OR (t.status='active' AND t.assigned_barber_id=?) ORDER BY t.created_at DESC");
    q.addBindValue("%" + QString::number(m_currentBarberId) + "%");
    q.addBindValue(m_currentBarberId);
    q.exec();
    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value(0); m["client_name"] = q.value(1); m["haircut"] = q.value(2);
        m["scheduled_time"] = q.value(3); m["total_price"] = q.value(4); m["status"] = q.value(5);
        list.append(m);
    }
    return list;
}

QVariantList Database::getBarberHistory()
{
    QVariantList list;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, client_name, total_price, completed_at FROM Tasks WHERE assigned_barber_id=? AND status='completed' ORDER BY completed_at DESC LIMIT 50");
    q.addBindValue(m_currentBarberId);
    q.exec();
    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value(0); m["client_name"] = q.value(1); m["total_price"] = q.value(2); m["completed_at"] = q.value(3);
        list.append(m);
    }
    return list;
}

QVariantList Database::getCashJournal() { return getPreviousJournals(); }

QVariantList Database::getPreviousJournals()
{
    QVariantList list;
    QSqlQuery q(m_db);
    q.exec("SELECT id, type, description, amount, barber_name, client_name, created_at, date FROM CashJournal ORDER BY created_at DESC");
    while (q.next()) {
        QVariantMap m;
        m["id"] = q.value(0); m["type"] = q.value(1); m["description"] = q.value(2);
        m["amount"] = q.value(3); m["barber_name"] = q.value(4); m["client_name"] = q.value(5);
        m["created_at"] = q.value(6); m["date"] = q.value(7);
        list.append(m);
    }
    return list;
}

void Database::startNewDay() { loadTable("CashJournal"); }

double Database::getCurrentDayTotal()
{
    QString today = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QSqlQuery q(m_db);
    q.prepare("SELECT SUM(amount) FROM CashJournal WHERE date=?");
    q.addBindValue(today);
    if (q.exec() && q.next()) return q.value(0).toDouble();
    return 0;
}

int Database::getTodayClientsCount()
{
    QString today = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QSqlQuery q(m_db);
    q.prepare("SELECT COUNT(DISTINCT client_name) FROM CashJournal WHERE date=?");
    q.addBindValue(today);
    if (q.exec() && q.next()) return q.value(0).toInt();
    return 0;
}

QString Database::clearCashJournal()
{
    QSqlQuery q(m_db);
    q.exec("DELETE FROM CashJournal");
    loadTable("CashJournal");
    return "Журнал очищен";
}

QString Database::exportToPDF(const QString &fileUrl)
{
    const QString localPath = QUrl(fileUrl).toLocalFile();
    if (localPath.isEmpty()) return "Ошибка";
    QPdfWriter writer(localPath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    QString html = "<html><head><meta charset='utf-8'><style>body{font-family:Segoe UI;}h1{color:#1e5f8c;}table{border-collapse:collapse;width:100%;}th,td{border:1px solid #7fa9c2;padding:8px;text-align:left;}th{background:#dff2ff;}</style></head><body><h1>" + currentTableDisplayName() + "</h1><table><tr>";
    for (int c = 0; c < m_tableModel->columnCount(); ++c) html += "<th>" + m_tableModel->headerData(c, Qt::Horizontal).toString() + "</th>";
    html += "</tr>";
    for (int r = 0; r < m_tableModel->rowCount(); ++r) {
        html += "<tr>";
        for (int c = 0; c < m_tableModel->columnCount(); ++c) html += "<td>" + m_tableModel->index(r, c).data().toString() + "</td>";
        html += "</tr>";
    }
    html += "</table></body></html>";
    QTextDocument doc; doc.setHtml(html); doc.print(&writer);
    return "PDF сохранен";
}

QString Database::exportToHTML(const QString &fileUrl)
{
    const QString localPath = QUrl(fileUrl).toLocalFile();
    if (localPath.isEmpty()) return "Ошибка";
    QFile file(localPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return "Ошибка записи";
    QString html = "<html><head><meta charset='utf-8'><style>body{font-family:Segoe UI;padding:24px;background:#f6fbff;}h1{color:#1e5f8c;}table{border-collapse:collapse;width:100%;background:white;}th,td{border:1px solid #7fa9c2;padding:8px;text-align:left;}th{background:#dff2ff;}</style></head><body><h1>" + currentTableDisplayName() + "</h1><table><tr>";
    for (int c = 0; c < m_tableModel->columnCount(); ++c) html += "<th>" + m_tableModel->headerData(c, Qt::Horizontal).toString() + "</th>";
    html += "</tr>";
    for (int r = 0; r < m_tableModel->rowCount(); ++r) {
        html += "<tr>";
        for (int c = 0; c < m_tableModel->columnCount(); ++c) html += "<td>" + m_tableModel->index(r, c).data().toString() + "</td>";
        html += "</tr>";
    }
    html += "</table></body></html>";
    file.write(html.toUtf8()); file.close();
    return "HTML сохранен";
}
