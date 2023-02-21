#ifndef RESTAPIMODULE_H
#define RESTAPIMODULE_H

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>

class RESTAPIModule : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(const QJsonObject &)> handleFunc;
    typedef std::function<void()> finishFunc;

    static const QString KEY_QNETWORK_REPLY_ERROR;
    static const QString KEY_CONTENT_NOT_FOUND;

    enum class Type {
        POST,
        GET,
        PATCH,
        DEL
    };

    explicit RESTAPIModule(QObject *parent = 0);

    void initRESTAPIModule(const QString& host, int port, QSslConfiguration *value);

    void sendRequest(int day, int month);

    void sendMulishGetRequest(const QString &apiStr,
                              const handleFunc &funcSuccess,
                              const handleFunc &funcError,
                              const finishFunc &funcFinish);

    QString getToken() const;
    void setToken(const QString &value);

private:
    static const QString httpTemplate;
    static const QString httpsTemplate;

    QString host;
    int port;
    QString token;
    QSslConfiguration *sslConfig;

    QString pathTemplate;

    QNetworkReply *reply;

    QByteArray variantMapToJson(QVariantMap data);

    QNetworkRequest createRequest(int day, int month);

    QNetworkReply *sendCustomRequest(QNetworkAccessManager *manager,
                                     QNetworkRequest &request,
                                     const QString &type,
                                     const QVariantMap &data);

    QJsonObject parseReply(QNetworkReply *reply);

    bool onFinishRequest(QNetworkReply *reply);

    void handleQtNetworkErrors(QNetworkReply *reply, QJsonObject &obj);
    QNetworkAccessManager *manager;

signals:
    void networkError();
    void dataRecieved(QString data);


public slots:
    void requestFinished();
};
#endif // RESTAPIMODULE_H
