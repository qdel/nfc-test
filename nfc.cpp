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
    filter.appendRecord(QNdefRecord::Mime, "nfc-test/msg");
    manager->setTargetAccessModes(QNearFieldManager::NdefReadTargetAccess); // doesn't help
    manager->registerNdefMessageHandler(filter, this, SLOT(handleMessage(QNdefMessage,QNearFieldTarget*)));
    qDebug() << __PRETTY_FUNCTION__ << "NFC available (test style)";
    connect(manager, SIGNAL(targetDetected(QNearFieldTarget*)),
            this, SLOT(targetDetected(QNearFieldTarget*)));
    connect(manager, SIGNAL(targetLost(QNearFieldTarget*)),
            this, SLOT(targetLost(QNearFieldTarget*)));
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

void nfc::stopNfc()
{
  qDebug() << __PRETTY_FUNCTION__ << "Nfc stop detection";
  manager->stopTargetDetection();
}

void nfc::targetDetected(QNearFieldTarget *target)
{
  qDebug() << __PRETTY_FUNCTION__ << "target detected:" << target;
  if (!target)
    return;
  connect(target, SIGNAL(ndefMessageRead(QNdefMessage)),
          this, SLOT(handlePolledNdefMessage(QNdefMessage)), Qt::UniqueConnection);
  target->readNdefMessages();
}
void nfc::targetLost(QNearFieldTarget *target)
{
  qDebug() << __PRETTY_FUNCTION__ << "target lost:" << target;
  if (target)
    target->deleteLater();
}

void nfc::handlePolledNdefMessage(QNdefMessage message)
{
    QNearFieldTarget *target = qobject_cast<QNearFieldTarget *>(sender());
    qDebug() << __PRETTY_FUNCTION__ << "New event";
    handleMessage(message, target);
    qDebug() << __PRETTY_FUNCTION__ << "New event end";
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
