#include "mainwindow.h"
#include <qstandardpaths.h>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QClipboard>
#include <QComboBox>
#include <QDebug>
#include <QUuid>
#include <QKeyEvent>
#include "capturearea.h"
#include "kdeconnect.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() {
    this->w.setupUi(this);
    auto devices = this->conn.listDevices();

    // Connect before adding iterms so first addItem will call currentTextChanged
    // and select device.
    connect(this->w.devicesCombo, &QComboBox::currentTextChanged, [this](const QString& s) {
        this->conn.selectDevice(s.split(" - ").at(0));
    });
    connect(&this->conn, &KDEConnect::photoReceived, [this](QMimeData* img) {
        auto cb = QGuiApplication::clipboard();
        cb->setMimeData(img);

        auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) +
                    QStringLiteral("/kdeconnect-applet-") + QUuid::createUuid().toString() + QStringLiteral(".jpeg");
        QFile out(path);
        out.open(QFile::WriteOnly);
        if (!out.isOpen()) {
            qDebug() << "Failed to create file:" << path << out.errorString();
            return;
        }
        if (out.write(img->data("image/jpeg")) == 0) {
            qDebug() << "Failed to write file:" << out.errorString();
        }
    });

    connect(this->w.inputArea, &CaptureArea::keyPress, &this->conn, &KDEConnect::relayKeyPress);
    connect(this->w.inputArea, &CaptureArea::paste, &this->conn, &KDEConnect::share);
    connect(this->w.requestPhotoBtn, &QPushButton::pressed, &this->conn, &KDEConnect::requestPhoto);

    for (auto it = devices.constBegin(); it != devices.constEnd(); ++it) {
        this->w.devicesCombo->addItem(it.key() + " - " + it.value());
    }
}

MainWindow::~MainWindow() {
}
