#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql>

class Database
{
public:
    Database();
    ~Database();
    void InitDB(QString connectionName = "defcname");
    void WriteToCache(const QString& text);
    QStringList ReadFromCache(int count);
    void WriteToComment(const QString& comment, uint textHash);
    QString ReadFromComment(uint textHash);
private:
    QSqlDatabase dbase;
};

#endif // DATABASE_H
