#include "database.h"

//QSqlDatabase Database::dbase;

Database::Database()
{
//    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
//    dbase.setDatabaseName("my_db.sqlite");
//    if (!dbase.open()) {
//        qDebug() << "Что-то пошло не так!";
//    }
//    else{
//        QSqlQuery a_query(dbase);

////        QString str = "CREATE TABLE my_table ("
////                      "id integer PRIMARY KEY NOT NULL, "
////                      "textid integer, "
////                      "comment TEXT"
////                      ");";
//        QString str = "CREATE TABLE my_table5 ("
//                      "id integer PRIMARY KEY NOT NULL, "
//                      "textid integer, "
//                      "comment TEXT"
//                ");";

//        if (!a_query.exec(str)) {
//            qDebug() << "Не удается создать таблицу комментариев";
//        }
//        /*else*/ {
//            QString str_insert = "INSERT INTO my_table5(id, textid, comment) "
//                                 "VALUES (%1, %2, '%3');";
//            str = str_insert.arg("2").arg("37").arg("text");

//            if (!a_query.exec(str)) {
//                qDebug() << "Данные не вставляются в таблицу комментариев";
//            }
//            /*else*/{//.....
//                if (!a_query.exec("SELECT * FROM my_table5")) {
//                    qDebug() << "Даже селект не получается, я пас.";
//                }
//                else{
//                    QSqlRecord rec = a_query.record();
//                    int number = 0,
//                            age = 0;
//                    QString address = "";

////                    if (a_query.first()){
////                        number = a_query.value(rec.indexOf("id")).toInt();
////                        age = a_query.value(rec.indexOf("textid")).toInt();
////                        address = a_query.value(rec.indexOf("comment")).toString();

////                        qDebug() << "id is " << number
////                                 << ". textid is " << age
////                                 << ". comment" << address;
////                    }
//                    while (a_query.next()) {
//                        number = a_query.value(rec.indexOf("id")).toInt();
//                        age = a_query.value(rec.indexOf("textid")).toInt();
//                        address = a_query.value(rec.indexOf("comment")).toString();

//                        qDebug() << "id is " << number
//                                 << ". textid is " << age
//                                 << ". comment" << address;
//                    }
//                }
//            }
//        }
//    }
}

Database::~Database()
{
    if (dbase.isOpen()) dbase.close();
}

void Database::InitDB(QString connectionName)
{
    if (!dbase.isOpen()) {
        dbase = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        dbase.setDatabaseName("my_db.sqlite");
        if (!dbase.open()){
            qDebug() << "InitDB: Невозможно установить соединение с базой данных";
        }
    }
}

void Database::WriteToCache(const QString &text)
{
//    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE", "my_db");
//    dbase.setDatabaseName("my_db.sqlite");
    if (!dbase.isOpen()) {
        qDebug() << "WriteToCache: Невозможно установить соединение с базой данных";
    }
    else {
        QSqlQuery a_query(dbase);
        QString str = "CREATE TABLE cache_table6 ("
                      "id integer PRIMARY KEY NOT NULL, "
                      "text TEXT"
                ");";

        if (!a_query.exec(str)) {
            qDebug() << "Не удается создать таблицу кэша";
        }
        /*else*/ {
            int hash = qHash(text);
            QString str_insert = "INSERT INTO cache_table6(id, text) "
                                 "VALUES (%1,'%2');";
            str = str_insert.arg(hash).arg(text);

            if (!a_query.exec(str)) {
                qDebug() << "Данные не вставляются в таблицу кэша" << str;
            }
        }
    }
}

QStringList Database::ReadFromCache(int count)
{
    QStringList data;
//    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE", "my_db");
//    dbase.setDatabaseName("my_db.sqlite");
    if (!dbase.isOpen()) {
        qDebug() << "ReadFromCache: Невозможно установить соединение с базой данных";
    }
    else {
        QSqlQuery a_query(dbase);
        if (!a_query.exec(QString("SELECT * FROM cache_table5 LIMIT %1").arg(count))) {
            qDebug() << "Даже селект не получается, я пас.";
        }
        else{
            QSqlRecord rec = a_query.record();
            int id = 0;
            QString text = "";

            while (a_query.next()) {
                id = a_query.value(rec.indexOf("id")).toInt();
                text = a_query.value(rec.indexOf("text")).toString();
                data.append(text);
                qDebug() << "id is " << id
                         << ". text" << text;
            }
        }
    }
    return data;
}

void Database::WriteToComment(const QString &comment, uint textHash)
{
    //QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE", "my_db");
    //dbase.setDatabaseName("my_db.sqlite");
    if (!dbase.isOpen()) {
        qDebug() << "WriteToComment: Невозможно установить соединение с базой данных";
    }
    else {
        QSqlQuery a_query(dbase);

        QString str = "CREATE TABLE comment_table5 ("
                      "textid integer PRIMARY KEY NOT NULL, "
                      "comment TEXT"
                ");";

        if (!a_query.exec(str)) {
            qDebug() << "Не удается создать таблицу комментариев";
        }
        /*else*/ {
            QString str_insert = "INSERT OR REPLACE INTO comment_table5(textid, comment) "
                                 "VALUES (%1, '%2');";
            str = str_insert.arg(textHash).arg(comment);

            if (!a_query.exec(str)) {
                qDebug() << "Данные не вставляются в таблицу комментариев";
            }
        }
    }
}

QString Database::ReadFromComment(uint textHash)
{
//    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE", "my_db");
//    dbase.setDatabaseName("my_db.sqlite");
    QString comment = "";
    if (!dbase.isOpen()) {
        qDebug() << "ReadFromComment: Невозможно установить соединение с базой данных";
    }
    else {
        QSqlQuery a_query(dbase);
        if (!a_query.exec(QString("SELECT comment FROM comment_table5 WHERE textid = %1").arg(textHash))) {
            qDebug() << "ReadFromComment error. Hash: " << textHash;
        }
        else{
            QSqlRecord rec = a_query.record();

            while (a_query.next()) {
                comment = a_query.value(rec.indexOf("comment")).toString();

                qDebug() << "comment" << comment;
            }
        }
    }
    return comment;
}
