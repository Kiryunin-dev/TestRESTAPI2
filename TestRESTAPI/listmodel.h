#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QAbstractItemModel>
#include <QStringList>
#include <qqml.h>

//class BackEnd : public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
//    QML_ELEMENT

//public:
//    explicit BackEnd(QObject *parent = nullptr);

//    QString userName();
//    void setUserName(const QString &userName);

//signals:
//    void userNameChanged();

//private:
//    QString m_userName;
//};

struct Record
{
    QString text;
    QString comment;
};

class TestModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        TextRole = Qt::UserRole + 1,
        CommentRole
    };

    TestModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void add(const QString &text, const QString &comment);

private:
    QVector<Record> listData;

public slots:
    void textRecieved(QString text);
    void addComment(QString commentText, int index);
};

#endif // BACKEND_H
