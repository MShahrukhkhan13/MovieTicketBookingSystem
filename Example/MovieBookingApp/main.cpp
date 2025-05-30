#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ApiClient.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ApiClient apiClient;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("apiClient", &apiClient);
    const QUrl qmlFileUrl = QUrl::fromLocalFile("MovieBookingApp/Main.qml");
    engine.load(qmlFileUrl);

    return app.exec();
}
