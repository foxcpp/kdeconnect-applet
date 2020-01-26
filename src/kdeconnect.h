#pragma once

#include <qwidget.h>
#include <memory>
#include <QtGlobal>
#include <QDBusConnection>
#include <QDBusInterface>

class KDEConnect : public QObject {
    Q_OBJECT
public:
    explicit KDEConnect();
    ~KDEConnect();

    QHash<QString, QString> listDevices();
    void selectDevice(const QString& deviceID);

public slots:
    void relayKeyPress(QKeyEvent* ev);
private:
    QString selectedId;
    std::unique_ptr<QDBusInterface> kbdIface;
    QDBusInterface kdeconnectIface;
};
