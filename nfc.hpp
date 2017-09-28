#ifndef NFC_H
#define NFC_H

#include <QObject>
#include <QtNfc>

class nfc : public QObject
{
    Q_OBJECT
public:
    explicit nfc(QObject *parent = 0);
    ~nfc();
    static nfc& getInstance();
signals:
    void read(QString);

public slots:
    void handleMessage(const QNdefMessage &message, QNearFieldTarget *target);
    void handlePolledNdefMessage(QNdefMessage message);
    void startNfc();
private:
    QNearFieldManager   *manager;
    QNdefFilter         filter;
    static nfc* instance;
};

#endif // NFC_H
