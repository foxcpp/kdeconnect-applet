#include "kdeconnect.h"
#include <qnamespace.h>
#include <stdexcept>
#include <QtDBus>
#include <QKeyEvent>

KDEConnect::KDEConnect() :
    kdeconnectIface("org.kde.kdeconnect.daemon", "/modules/kdeconnect",
                    "org.kde.kdeconnect.daemon", QDBusConnection::sessionBus()) {

    qDBusRegisterMetaType<QHash<QString, QString>>();
}

KDEConnect::~KDEConnect() {}

QHash<QString, QString> KDEConnect::listDevices() {
    QDBusReply<QHash<QString, QString>> reply = kdeconnectIface.call("deviceNames", QVariant(true), QVariant(true));
    if (!reply.isValid()) {
        throw std::runtime_error(reply.error().message().toStdString());
    }
    qDebug() << "Devices:" << reply.value();
    return reply.value();
}

void KDEConnect::relayKeyPress(QKeyEvent* ev) {
    if (this->selectedId.isEmpty()) {
        return;
    }

    QDBusError err = this->kbdIface->call("sendQKeyEvent", QHash<QString, QVariant>{
        {"accepted", false},
        {"key", ev->key()},
        {"modifiers", int(ev->modifiers())},
        {"nativeScanCode", ev->nativeScanCode()},
        {"text", ev->text()},
    });
    if (err.isValid()) {
        throw std::runtime_error(err.message().toStdString());
    }
}

void KDEConnect::selectDevice(const QString& id) {
    qDebug() << "Selected" << id;
    this->selectedId = id;
    this->kbdIface.reset(new QDBusInterface(
            "org.kde.kdeconnect.daemon",
            QStringLiteral("/modules/kdeconnect/devices/") + id + "/remotekeyboard",
            "org.kde.kdeconnect.device.remotekeyboard", QDBusConnection::sessionBus()));
}
