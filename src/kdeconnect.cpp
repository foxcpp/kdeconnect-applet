#include "kdeconnect.h"
#include <stdexcept>
#include <QtDBus>
#include <QKeyEvent>
#include <QDebug>

KDEConnect::KDEConnect()
    : kdeconnectIface("org.kde.kdeconnect.daemon",
                      "/modules/kdeconnect",
                      "org.kde.kdeconnect.daemon",
                      QDBusConnection::sessionBus()) {
    qDBusRegisterMetaType<QHash<QString, QString>>();
}

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

    QDBusError err = this->kbdIface->call("sendQKeyEvent",
                                          QHash<QString, QVariant>{
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

void KDEConnect::requestPhoto() {
    auto path = QDir::tempPath() + QStringLiteral("/kdeconnect-applet-") + QUuid::createUuid().toString() +
                QStringLiteral(".jpeg");

    QDBusError err = this->photoIface->call("requestPhoto", path);
    if (err.isValid()) {
        throw std::runtime_error(err.message().toStdString());
    }

    qDebug() << "Requested photo for path" << path << "from" << this->selectedId;
}

void KDEConnect::photoReceivedSlot(const QString& path) {
    QFile f(path);
    f.open(QFile::ReadOnly);
    if (!f.isOpen()) {
        throw std::runtime_error(f.errorString().toStdString());
    }
    auto mime = new QMimeData;
    auto blob = f.readAll();
    if (blob.size() == 0) {
        throw std::runtime_error(f.errorString().toStdString());
    }
    mime->setData("image/jpeg", blob);
    qDebug() << "Received photo with path" << path;

    if (!f.remove()) {
        qDebug() << "Failed to remove file:" << f.errorString();
    }

    emit photoReceived(mime);
}

void KDEConnect::selectDevice(const QString& id) {
    qDebug() << "Selected" << id;
    this->selectedId = id;
    auto bus = QDBusConnection::sessionBus();

    this->kbdIface.reset(new QDBusInterface("org.kde.kdeconnect.daemon",
                                            QStringLiteral("/modules/kdeconnect/devices/") + id + "/remotekeyboard",
                                            "org.kde.kdeconnect.device.remotekeyboard",
                                            bus));
    this->photoIface.reset(new QDBusInterface("org.kde.kdeconnect.daemon",
                                              QStringLiteral("/modules/kdeconnect/devices/") + id + "/photo",
                                              "org.kde.kdeconnect.device.photo",
                                              bus));

    bool ok = bus.connect(this->photoIface->service(),
                          this->photoIface->path(),
                          this->photoIface->interface(),
                          "photoReceived",
                          "s",
                          this,
                          SLOT(photoReceivedSlot(QString)));
    if (!ok) {
        qDebug() << "Signal connection failed";
    }
}
