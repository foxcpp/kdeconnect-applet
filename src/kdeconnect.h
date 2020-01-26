#pragma once

#include <QWidget>
#include <memory>
#include <QtGlobal>
#include <QMimeData>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QFile>

class KDEConnect : public QObject {
    Q_OBJECT
public:
    explicit KDEConnect();

    QHash<QString, QString> listDevices();
    void selectDevice(const QString& deviceID);

    void requestPhoto();

signals:
    void photoReceived(QMimeData*);
public slots:
    void relayKeyPress(QKeyEvent* ev);
private slots:
    void photoReceivedSlot(const QString&);

private:
    QString selectedId;
    std::unique_ptr<QDBusInterface> kbdIface;
    std::unique_ptr<QDBusInterface> photoIface;
    QDBusInterface kdeconnectIface;
};
