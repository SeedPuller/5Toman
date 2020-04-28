#include "header/database.h"

Database::Database(const QString& hostname, const QString& dbname, QObject* parent)
    : QObject(parent), hostname(hostname), dbname(dbname)
{
    connectToDatabase();
    query = new QSqlQuery(this->db);

}

bool Database::connectToDatabase() {
    if(!QFile("./" + this->dbname).exists()) {
//        qDebug() << "db not exists.";
        this->db_exist_flag = true;
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(hostname);
    db.setDatabaseName("./" + this->dbname);
    if (db.open()) {
        return true;
    }
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
    if (params.size() == 0) {
        return query.exec(querystr);
    }
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

bool Database::isFirstInit() const
{
    return db_exist_flag;
}
