#include "header/database.h"

Database::Database(const char* hostname, const char* dbname, QObject* parent)
: QObject(parent), hostname(hostname), dbname(dbname)
{
    connectToDatabase();
    query = new QSqlQuery(this->db);

}

void Database::connectToDatabase() {
    if(!QFile("./" + this->dbname).exists()) {
        qDebug() << "db not exists. restoring...";
        this->restoreDatabase();
    } else {
        this->openDatabase();
    }
}

bool Database::restoreDatabase() {
    if(this->openDatabase()) {
        if(this->createTable()) {
//            qDebug() << "opened successfully";
            return true;
        } else {
          return false;
        }
    }
    return false;
}

bool Database::openDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(hostname);
    db.setDatabaseName("./" + this->dbname);
    if (db.open()) {
        return true;
    }
    return false;
}

bool Database::createTable() {
    QSqlQuery query;
    QString create_query{"CREATE TABLE " DB_TABLE " ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "fullname VARCHAR(255) NOT NULL, "
                          "debt VARCHAR(255) NOT NULL,"
                          "picurl VARCHAR(255) NOT NULL"
                          ")"
                        };
    if (query.exec(create_query)) {
        return true;
    }
    qDebug() << "error creating table: " DB_TABLE;
    qDebug() << query.lastError().text();
    return false;
}

bool Database::execute(const QString &querystr, const QStringList *params)
{
    if (params == nullptr) {
        return this->query->exec(querystr);
    }

    this->query->prepare(querystr);

    for (QString p: *params) {
        this->query->addBindValue(p);
    }

    return this->query->exec();
}

bool Database::execute(const QString &querystr, const QVariantList& params)
{
    this->query->prepare(querystr);
    for (QVariant p: params) {
        QVariant::Type type = p.type();
        if (type == QVariant::Int) {
            this->query->addBindValue(p.toInt());
        } else if (type == QVariant::Double) {
            this->query->addBindValue(p.toDouble());
        } else {
            this->query->addBindValue(p.toString());
        }
    }
    return this->query->exec();
}

bool Database::executeWithoutFetch(const QString &querystr, const QVariantList &params) const
{
    QSqlQuery query;
    query.prepare(querystr);
    for (QVariant p: params) {
        QVariant::Type type = p.type();
        if (type == QVariant::Int) {
            query.addBindValue(p.toInt());
        } else if (type == QVariant::Double) {
            query.addBindValue(p.toDouble());
        } else {
            query.addBindValue(p.toString());
        }
    }
    return query.exec();
}

int Database::fetch(QVector<QVariantList>& rows, int columncounter)
{
    QVariantList row;
    int vectorsize{rows.size()};
    if (columncounter < 1) {
        return 0;
    }
    while (this->query->next()) {
        for (int i{0}; i < columncounter; ++i) {
            row.append(this->query->value(i));
        }
        rows.append(row);
        row.clear();
    }
    return rows.size() - vectorsize;
}

QString Database::getError() const
{
    return this->query->lastError().text();
}
