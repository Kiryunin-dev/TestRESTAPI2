#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database
{
public:
    Database();
    void WriteToCacheDB(const QString& text);
};

#endif // DATABASE_H
