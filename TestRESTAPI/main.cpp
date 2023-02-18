#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");

    TestModel testModel;

    QQmlApplicationEngine engine;
    engine.setInitialProperties({{"model1", QVariant::fromValue(&testModel)}});
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QQuickView view;
        view.setResizeMode(QQuickView::SizeRootObjectToView);
        view.setInitialProperties({{"model", QVariant::fromValue(&testModel)}});
    //![0]
        view.setSource(QUrl("qrc:view.qml"));
        view.show();


    return app.exec();
}
