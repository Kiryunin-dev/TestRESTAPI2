#include "database.h"
#include <QtSql>

Database::Database()
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName("my_db.sqlite");
    if (!dbase.open()) {
        qDebug() << "Что-то пошло не так!";
    }
    else{
        QSqlQuery a_query(dbase);

//        QString str = "CREATE TABLE my_table ("
//                      "id integer PRIMARY KEY NOT NULL, "
//                      "textid integer, "
//                      "comment TEXT"
//                      ");";
        QString str = "CREATE TABLE my_table5 ("
                      "id integer PRIMARY KEY NOT NULL, "
                      "textid integer, "
                      "comment TEXT"
                ");";

        if (!a_query.exec(str)) {
            qDebug() << "Не удается создать таблицу комментариев";
        }
        /*else*/ {
            QString str_insert = "INSERT INTO my_table5(id, textid, comment) "
                                 "VALUES (%1, %2, '%3');";
            str = str_insert.arg("2").arg("37").arg("text");

            if (!a_query.exec(str)) {
                qDebug() << "Кажется данные не вставляются, проверьте дверь, может она закрыта?";
            }
            /*else*/{//.....
                if (!a_query.exec("SELECT * FROM my_table5")) {
                    qDebug() << "Даже селект не получается, я пас.";
                }
                else{
                    QSqlRecord rec = a_query.record();
                    int number = 0,
                            age = 0;
                    QString address = "";

//                    if (a_query.first()){
//                        number = a_query.value(rec.indexOf("id")).toInt();
//                        age = a_query.value(rec.indexOf("textid")).toInt();
//                        address = a_query.value(rec.indexOf("comment")).toString();

//                        qDebug() << "id is " << number
//                                 << ". textid is " << age
//                                 << ". comment" << address;
//                    }
                    while (a_query.next()) {
                        number = a_query.value(rec.indexOf("id")).toInt();
                        age = a_query.value(rec.indexOf("textid")).toInt();
                        address = a_query.value(rec.indexOf("comment")).toString();

                        qDebug() << "id is " << number
                                 << ". textid is " << age
                                 << ". comment" << address;
                    }
                }
            }
        }
    }
}

void Database::WriteToCacheDB(const QString &text)
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName("my_db.sqlite");
    if (!dbase.open()) {
        qDebug() << "Что-то пошло не так!";
    }
    else{
        QSqlQuery a_query(dbase);
        QString str = "CREATE TABLE cache_table5 ("
                      "id integer PRIMARY KEY NOT NULL, "
                      "text TEXT"
                ");";

        if (!a_query.exec(str)) {
            qDebug() << "Не удается создать таблицу кэша";
        }
        /*else*/ {
            int hash = qHash(text);
            QString str_insert = "INSERT INTO my_table5(id, textid, comment) "
                                 "VALUES (%1,'%2');";
            str = str_insert.arg(hash).arg(text);

            if (!a_query.exec(str)) {
                qDebug() << "Кажется данные не вставляются, проверьте дверь, может она закрыта?";
            }
            /*else*/{//.....
                if (!a_query.exec("SELECT * FROM my_table5")) {
                    qDebug() << "Даже селект не получается, я пас.";
                }
                else{
                    QSqlRecord rec = a_query.record();
                    int id = 0;
                    QString text = "";

                    while (a_query.next()) {
                        id = a_query.value(rec.indexOf("id")).toInt();
                        text = a_query.value(rec.indexOf("text")).toString();

                        qDebug() << "id is " << id
                                 << ". text" << text;
                    }
                }
            }
        }
    }
}
