#include "restapimodule.h"

const QString RESTAPIModule::httpTemplate = "http://numbersapi.com/%1/%2/date";
const QString RESTAPIModule::httpsTemplate = "https://numbersapi.com/%1/%2/date";
const QString RESTAPIModule::KEY_QNETWORK_REPLY_ERROR = "QNetworkReplyError";
const QString RESTAPIModule::KEY_CONTENT_NOT_FOUND = "ContentNotFoundError";

RESTAPIModule::RESTAPIModule(QObject *parent) : QObject(parent), reply(nullptr)
{
    manager = new QNetworkAccessManager(this);
}

void RESTAPIModule::initRESTAPIModule(const QString &host, int port, QSslConfiguration *value)
{
    this->host = host;
    this->port = port;
    sslConfig = value;
    if (sslConfig != nullptr)
        pathTemplate = httpsTemplate;
    else
        pathTemplate = httpTemplate;
}

void RESTAPIModule::sendRequest(int day, int month)
{
    QNetworkRequest request = createRequest(day, month);

//    QNetworkReply *reply;
    Type type = Type::GET;
    switch (type) {
    case Type::POST: {
        //QByteArray postDataByteArray = variantMapToJson(data);
        //reply = manager->post(request, postDataByteArray);
        break;
    } case Type::GET: {
        reply = manager->get(request);
        break;
    } case Type::DEL: {
//        if (data.isEmpty())
//            reply = manager->deleteResource(request);
//        else
//            reply = sendCustomRequest(manager, request, "DELETE", data);
        break;
    } case Type::PATCH: {
        //reply = sendCustomRequest(manager, request, "PATCH", data);
        break;
    } default:
        reply = nullptr;
        Q_ASSERT(false);
    }

//    connect(reply, &QNetworkReply::finished, this,
//            [this, funcSuccess, funcError, reply]() {
//        QJsonObject obj = parseReply(reply);

//        if (onFinishRequest(reply)) {
//            if (funcSuccess != nullptr)
//                funcSuccess(obj);
//        } else {
//            if (funcError != nullptr) {
//                handleQtNetworkErrors(reply, obj);
//                funcError(obj);
//            }
//        }
//        reply->close();
//        reply->deleteLater();
//    } );
    connect(reply, &QNetworkReply::finished, this, requestFinished);
}

void RESTAPIModule::sendMulishGetRequest(const QString &apiStr, //а ничего что здесь нигде не проверяется func != nullptr?
                                     const handleFunc &funcSuccess,
                                     const handleFunc &funcError,
                                     const finishFunc &funcFinish)
{
    QNetworkRequest request = createRequest(11,11);
    //    QNetworkReply *reply;
    qDebug() << "GET REQUEST " << request.url().toString() << "\n";
    auto reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, funcSuccess, funcError, funcFinish, reply]() {
        QJsonObject obj = parseReply(reply);
        if (onFinishRequest(reply)) {
            if (funcSuccess != nullptr)
                funcSuccess(obj);
            QString nextPage = obj.value("next").toString();
            if (!nextPage.isEmpty()) {
                QStringList apiMethodWithPage = nextPage.split("api/");
                sendMulishGetRequest(apiMethodWithPage.value(1),
                                     funcSuccess,
                                     funcError,
                                     funcFinish
                                     );
            } else {
                if (funcFinish != nullptr)
                    funcFinish();
            }
        } else {
            handleQtNetworkErrors(reply, obj);
            if (funcError != nullptr)
                funcError(obj);
        }
        reply->close();
        reply->deleteLater();
    });
}


QString RESTAPIModule::getToken() const
{
    return token;
}

void RESTAPIModule::setToken(const QString &value)
{
    token = value;
}

QByteArray RESTAPIModule::variantMapToJson(QVariantMap data)
{
    QJsonDocument postDataDoc = QJsonDocument::fromVariant(data);
    QByteArray postDataByteArray = postDataDoc.toJson();

    return postDataByteArray;
}

QNetworkRequest RESTAPIModule::createRequest(int day, int month)
{
    QNetworkRequest request;
    QString url = pathTemplate.arg(month).arg(day);
    request.setUrl(QUrl(url));
    request.setRawHeader("Content-Type","application/json");
    if(!token.isEmpty())
        request.setRawHeader("Authorization",QString("token %1").arg(token).toUtf8());
    if (sslConfig != nullptr)
        request.setSslConfiguration(*sslConfig);

    return request;
}

QNetworkReply* RESTAPIModule::sendCustomRequest(QNetworkAccessManager* manager,
                                            QNetworkRequest &request,
                                            const QString &type,
                                            const QVariantMap &data)
{
    request.setRawHeader("HTTP", type.toUtf8());
    QByteArray postDataByteArray = variantMapToJson(data);
    QBuffer *buff = new QBuffer;
    buff->setData(postDataByteArray);
    buff->open(QIODevice::ReadOnly);
    QNetworkReply* reply =  manager->sendCustomRequest(request, type.toUtf8(), buff);
    buff->setParent(reply);
    return reply;
}

QJsonObject RESTAPIModule::parseReply(QNetworkReply *reply)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    auto replyText = reply->readAll();
    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            jsonObj  = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}

bool RESTAPIModule::onFinishRequest(QNetworkReply *reply)
{
    auto replyError = reply->error();
    if (replyError == QNetworkReply::NoError ) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if ((code >=200) && (code < 300)) {
            return true;
        }
    }
    return false;
}

void RESTAPIModule::handleQtNetworkErrors(QNetworkReply *reply, QJsonObject &obj)
{
    auto replyError = reply->error();
    if (!(replyError == QNetworkReply::NoError ||
          replyError == QNetworkReply::ContentNotFoundError ||
          replyError == QNetworkReply::ContentAccessDenied ||
          replyError == QNetworkReply::ProtocolInvalidOperationError
          ) ) {
        qDebug() << reply->error();
        obj[KEY_QNETWORK_REPLY_ERROR] = reply->errorString();
    } else if (replyError == QNetworkReply::ContentNotFoundError)
        obj[KEY_CONTENT_NOT_FOUND] = reply->errorString();
}

void RESTAPIModule::requestFinished()
{
//     qDebug() << "requestFinished";
    if (reply){
        QJsonObject obj = parseReply(reply);

        if (onFinishRequest(reply)) {
//            qDebug() << obj["text"].toString();
//            qDebug() << obj["year"].toInt();
//            for(const auto &jsonElem: obj){
//                qDebug()  << jsonElem.toString();
//            }
            emit dataRecieved(obj["text"].toString());
        } else {
            handleQtNetworkErrors(reply, obj);
        }
        reply->close();
        reply->deleteLater();
    }
}
