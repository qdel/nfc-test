#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "nfc.hpp"

int main(int argc, char *argv[])
{
    qDebug() << "START OF MAIN!";
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //nfc::getInstance().startNfc();
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    qDebug() << "Loaded qml!";
    if (engine.rootObjects().isEmpty())
        return -1;
    int x = app.exec();
    qDebug() << "END OF MAIN";
    return x;
}
