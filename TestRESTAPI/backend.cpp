#include "backend.h"

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

    return m_data.size();
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case ColorRole:
        return "";
    case TextRole:
        return m_data.at(index.row());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TestModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[ColorRole] = "date";
    roles[TextRole] = "textt";

    return roles;
}

void TestModel::add(const QString &text)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(text);
    endInsertRows();

    //m_data[0] = QString("Size: %1").arg(m_data.size());
    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}

void TestModel::textRecieved(QString text)
{
    add(text);
}
