#include "nfc.hpp"
#include <QDebug>
#include <QtNfc>
#include <QGuiApplication>

nfc* nfc::instance = NULL;

nfc&    nfc::getInstance()
{
    if (nfc::instance == NULL)
        nfc::instance = new nfc(QGuiApplication::instance());
    return *(nfc::instance);
}

nfc::nfc(QObject *parent) : QObject(parent)
{
    nfc::instance = this;
    manager = new QNearFieldManager(this);
    if (!manager->isAvailable()) {
        qWarning() << "NFC not available";
        return;
    }
    filter.appendRecord(QNdefRecord::Mime, "rhoflat/msg");
    manager->registerNdefMessageHandler(filter, this, SLOT(handleMessage(QNdefMessage,QNearFieldTarget*)));
    qDebug() << "NFC available (test style)";
}

nfc::~nfc()
{
    manager->stopTargetDetection();
    delete manager;
    manager = NULL;
}

void nfc::startNfc()
{
    qDebug() << "Started nfc detection";
    manager->startTargetDetection();
}

void nfc::handlePolledNdefMessage(QNdefMessage message)
{
    QNearFieldTarget *target = qobject_cast<QNearFieldTarget *>(sender());
    qDebug() << "New event";
    handleMessage(message, target);
    qDebug() << "New event end";
}

void nfc::handleMessage(const QNdefMessage &message, QNearFieldTarget *target)
{
    Q_UNUSED(target);

    qDebug() << "message : " << message.toByteArray() << message.size() << target;
    foreach (const QNdefRecord &record, message) {
        qDebug() << "NFC id=>" << record.id();
        qDebug() << "NFC type=>" << record.type();
        qDebug() << "NFC payload=>" << record.payload();
        qDebug() << "NFC typenameformat=>" << record.typeNameFormat();
    }

}
