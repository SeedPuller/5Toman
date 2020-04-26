#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "database.h"

#define DB_HOSTNAME    "localhost"
#define DB_NAME        "modeldb.db"


class ViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum {
        idRole = Qt::UserRole + 1,
        fnameRole,
        debtRole,
        picRole,
        ROLE_END
    };

    explicit ViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    bool insertFirst(
                     const QString& name,
                     const QString& debt,
                     const QString& picurl,
                     const QModelIndex &parent = QModelIndex()
                    );

    bool remove(int index, const QModelIndex &parent = QModelIndex());
    bool updateDB(int index);
    bool setPic(int index, QString url);
    bool setPicInDB(int id, QString url);

private:
    QVector<QVariantList> vlist;
    const QList<QByteArray> columns{"id", "fullname", "debt", "picurl"};
    Database db{DB_HOSTNAME, DB_NAME};
    int lastid;

    int select(
                const QList<QByteArray>& columns,
                const QByteArray& order = nullptr,
                bool ascend = true,
                bool limit = false
                );

    bool removeRow(const int id);
    bool insertRow(const QVariantList& data);
    bool insertRow(const QString& fname, const QString& debt, const QString& picurl);
    bool changeRow(QStringList* values);
};

#endif // VIEWMODEL_H
