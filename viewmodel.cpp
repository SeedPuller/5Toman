#include "viewmodel.h"
#include <QDebug>
ViewModel::ViewModel(QObject *parent)
    : QAbstractListModel(parent), lastid{-1}
{
    select(this->columns, "id", false);
//    foreach (QVariantList var, vlist) {
//        qDebug() << var[0].toInt() << "\t" << var[1].toString();
//    }
}

int ViewModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return vlist.size();
}

QVariant ViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || vlist.size() <= 0)
        return QVariant();

    QVariantList temp = vlist.at(index.row());
    switch (role) {
        case idRole:
            return temp[0];
        case fnameRole:
            return temp[1];
        case debtRole:
            return temp[2];
        case picRole:
            return temp[3];
//        case editRole:
//            return QVariant(false);
    }
    return QVariant();
}


bool ViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int indexrow = index.row();
    if (data(index, role) != value && vlist.size() > indexrow) {

        switch (role) {

            case fnameRole:
                vlist[indexrow][1] = value.toString();
            break;
            case debtRole:
                vlist[indexrow][2] = value.toString();
            break;
            case picRole:
                vlist[indexrow][3] = value.toString();
            break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ViewModel::roleNames() const
{
    QHash<int, QByteArray> roles; 
    int column_number{0};
    for (int i{idRole}; i != ROLE_END; ++i, ++column_number) {
        roles.insert(i, this->columns[column_number]);
    }
    return roles;
}

bool ViewModel::insertFirst(const QString& name, const QString& debt, const QString& picurl, const QModelIndex &parent)
{
    int rowsaffected{0};
    bool insertresult{false};
    beginInsertRows(parent, 0, 0);
    insertresult = insertRow(name, debt, picurl);
    if (lastid == -1) {
        if (!insertresult) {
//            qDebug() << "insertFirst not ok";
            return false;
        }
        QVector<QVariantList> result;
        db.execute("SELECT id FROM " DB_TABLE " ORDER BY id DESC LIMIT 1");
        rowsaffected = db.fetch(result, 1);
        if (rowsaffected <= 0) {
            qDebug() << db.getError();
        }else {
            lastid = result[0][0].toInt();
        }
    } else {
        ++lastid;
    }

    QVariantList temp;
    temp.append(lastid);
    temp.append(name);
    temp.append(debt);
    temp.append(picurl);
    vlist.push_front(temp);
//    qDebug() << "added to vlist id: " << lastid;
    endInsertRows();
    return true;
}


bool ViewModel::remove(int index, const QModelIndex& parent) {
//        qDebug() << "removing index number: " << index;
        beginRemoveRows(parent, index, index);
        if (!removeRow(vlist[index][0].toInt())) {
            qDebug() << db.getError();
        }
        vlist.removeAt(index);
        endRemoveRows();
        return true;
}


int ViewModel::select(const QList<QByteArray>& columns,
                       const QByteArray& order,
                       bool ascend,
                       bool limit
                      )
{
    int numberofrows{0};
    QString querystr = "SELECT " + columns.join(",") + " FROM " DB_TABLE;

    if (limit) {
        querystr += " LIMIT " + QString(limit);
    }

    if (order != nullptr) {
        querystr += " ORDER BY " + order;
        if (ascend) {
            querystr += " ASC";
        } else {
            querystr += " DESC";
        }
    }

    if (!db.execute(querystr)) {
        qDebug() << "select failed";
        qDebug() << db.getError();
        return -1;
    }

    numberofrows = db.fetch(vlist, columns.size());

    return numberofrows;
}

bool ViewModel::removeRow(const int id) {
    QVariantList params;
    params.append(id);
    QString query{"DELETE FROM " DB_TABLE " WHERE id = ?"};
    if (db.executeWithoutFetch(query, params)) {
//        qDebug() << "removed: " << id;
        return true;
    }
    qDebug() << "error deleting id: " << id;
    qDebug() << db.getError();
    return false;
}

bool ViewModel::insertRow(const QVariantList& data) {
    QString query{"INSERT INTO " DB_TABLE " VALUES (NULL, ?, ?, ?)"};
    if (db.executeWithoutFetch(query, data)) {
//        qDebug() << "added to db successfully";
        return true;
    }
    qDebug() << "Error inserting to " DB_TABLE;
    qDebug() << db.getError();
    return false;
}

bool ViewModel::insertRow(const QString& fname, const QString& debt, const QString& picurl) {
    QVariantList data;
    data.append(fname);
    data.append(debt);
    data.append(picurl);
    if (this->insertRow(data)) {
        return true;
    }
    return false;
}

bool ViewModel::updateDB(int index)
{
    QVariantList& temp = vlist[index];
    QStringList valuelist;

    /*
     * size -1 because we dont want picurl column to edit.
     * editing picurl in database is 'setPicInDB' function job.
     */
    for (int i{0}; i < temp.size() - 1; ++i) {
        valuelist.append(temp[i].toString());
    }
    return this->changeRow(&valuelist);
}

/**
 *  this function set pic in model vector and then call setPicInDB to update picture
 *  in the database
 */
bool ViewModel::setPic(int index, QString url)
{
    QVariantList datas = vlist[index];
    if (datas[3].toString() == url) {
        return true;
    }
    QModelIndex indexmodel = QAbstractItemModel::createIndex(index, 0);
    if (setData(indexmodel, url, picRole)) {
        setPicInDB(datas[0].toInt(), url);
        emit
    }
    return false;
}

bool ViewModel::changeRow(QStringList* values)
{
//    return true;
    QString query{"UPDATE " DB_TABLE " SET "};
    QVariantList params;
    int columnid{1};

    /**
     * this function is not for updating picurl in database.
     * editing picurl in database is 'setPicInDB' function job.
     */
    for (; columnid != this->columns.size() - 2; ++columnid) {
        query += this->columns[columnid] + " = ?, ";
        params.append(values->at(columnid));
    }
    query += this->columns[columnid] + " = ? WHERE id = ?";
    params.append(values->at(columnid)); // add last column for update
    params.append(values->at(0)); // add id for finding a row

    if (db.executeWithoutFetch(query, params)) {
        return true;
    }

    qDebug() << db.getError();
    return false;
}

bool ViewModel::setPicInDB(int id, QString &url)
{
    QString query{"UPDATE " DB_TABLE " SET picurl = ? WHERE id = ?"};
    QVariantList params{id, url};
    return db.executeWithoutFetch(query, params);
}
