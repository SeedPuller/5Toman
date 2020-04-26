#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>
#include <QVector>

#define DB_TABLE       "debtors"

struct ModelStruct {
    int id;
    QString name;
    QString debt;
    QString picurl;
};

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(const char* hostname, const char* dbname, QObject* parent = nullptr);
    ~Database() = default;

    bool execute(const QString& query, const QStringList* params = nullptr);
    bool execute(const QString &querystr, const QVariantList &params);
    bool executeWithoutFetch(const QString &querystr, const QVariantList &params) const;
    int fetch(QVector<QVariantList>& rows, int columncounter);
    QString getError() const;
private:
    QSqlDatabase db;
    QSqlQuery* query;

private:
    void connectToDatabase();
    bool openDatabase();
    bool restoreDatabase();
    bool createTable();
//    void closeDatabase();

    QString hostname;
    QString dbname;
};

#endif // DATABASE_H
