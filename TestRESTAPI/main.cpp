#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include <QJsonObject>

#include "backend.h"
#include "RESTAPIModule.h"
#include "database.h"

void handleFunction(const QJsonObject &obj)
{
    qDebug() << "handleFunction";
    for(const auto &jsonElem: obj){
        qDebug() << jsonElem.toString();
    }
}

void errorFunction(const QJsonObject &obj)
{
    qDebug() << "errorFunction";
    for(const auto &jsonElem: obj){
        qDebug() << jsonElem.toString();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");

    Database db;
    TestModel testModel;
    RESTAPIModule *raModule;

    for (int i = 1; i < 28; i++){
        raModule = new RESTAPIModule();
        raModule->initRESTAPIModule("", 0, nullptr);
        raModule->sendRequest(i, 2);
        QObject::connect(raModule, &RESTAPIModule::dataRecieved, &testModel, &TestModel::textRecieved);
    }

//    for (int i = 1; i < 30; i++){
//        testModel.add("");
//    }


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mymodel", &testModel);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

//    QQuickView view;
//        view.setResizeMode(QQuickView::SizeRootObjectToView);
//        view.setInitialProperties({{"model", QVariant::fromValue(&testModel)}});
//    //![0]
//        view.setSource(QUrl("qrc:view.qml"));
//        view.show();


    return app.exec();
}
