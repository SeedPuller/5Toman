#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "database.h"

#define DB_HOSTNAME    "localhost"
#define DB_NAME        "modeldb.db"
#define DB_TABLE       "debtors"


class ViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /*
     * roles for each column. ROLE_END is a flag for iteration convenience
     */
    enum {
        idRole = Qt::UserRole + 1,
        fnameRole,
        debtRole,
        picRole,
        ROLE_END
    };

    explicit ViewModel(QObject *parent = nullptr);
    // return number of data's
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // return data by role and index
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // set data by role and index
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    // insert a row into database then sync inserted row with "vlist".
    bool insertFirst(
                     const QString& name,
                     const QString& debt,
                     const QString& picurl,
                     const QModelIndex &parent = QModelIndex()
                    );
    // remove an element by index
    bool remove(int index, const QModelIndex &parent = QModelIndex());
    // update database for given row. index is equal to "id" in columns
    bool updateDB(int index);
    //  set pic in model vector and then call setPicInDB to update picture in the database
    bool setPic(int index, QString url);
    // update a "picurl" in database
    bool setPicInDB(int id, QString url);

private:
    // last inserted row's id
    int lastid;
    QVector<QVariantList> vlist;
    // list of columns
    const QList<QByteArray> columns{"id", "fullname", "debt", "picurl"};
    Database db{QString(DB_HOSTNAME), QString(DB_NAME)};

    // utility to "select" statements in sql.
    int select(
                const QList<QByteArray>& columns,
                const QByteArray& order = nullptr,
                bool ascend = true,
                bool limit = false
                );
    // utility to create table in database initialization
    bool createDbTable() const;
    // remove a row in database by "id"
    bool removeRow(const int id);
    // insert a row in database
    bool insertRow(const QVariantList& data);
    // insert a row in database.
    bool insertRow(const QString& fname, const QString& debt, const QString& picurl);
    // change a row in database. this function is called via updateDB. this function does NOT change picurl column
    // this strategy must be changed because its error-prone.
    bool changeRow(QStringList* values);
};

#endif // VIEWMODEL_H
