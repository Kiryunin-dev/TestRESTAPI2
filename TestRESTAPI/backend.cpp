#include "backend.h"
#include "database.h"

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    setUserName("Test");
}

QString BackEnd::userName()
{
    return m_userName;
}

void BackEnd::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

TestModel::TestModel(QObject *parent):
    QAbstractListModel(parent)
{
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return listData.size();
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case TextRole:
        return listData.at(index.row()).text;
    case CommentRole:
        return listData.at(index.row()).comment;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TestModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[TextRole] = "textt";
    roles[CommentRole] = "comment";

    return roles;
}

void TestModel::add(const QString &text, const QString &comment)
{
    beginInsertRows(QModelIndex(), listData.size(), listData.size());
    Record r{text, comment};
    listData.push_back(r);
    endInsertRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void TestModel::textRecieved(QString text)
{
    Database db;
    db.InitDB(text);
    QString commentText = db.ReadFromComment(qHash(text));
    add(text, commentText);
    db.WriteToCache(text);
}

void TestModel::addComment(QString commentText, int index)
{
    Record r{listData.at(index).text, commentText};
    listData.replace(index, r);

    Database db;
    db.InitDB("addComment");
    db.WriteToComment(commentText, qHash(r.text));

    QModelIndex modelIndex = createIndex(index, 0, static_cast<void *>(0));
    emit dataChanged(modelIndex, modelIndex);
    qDebug() << commentText << index;
}
