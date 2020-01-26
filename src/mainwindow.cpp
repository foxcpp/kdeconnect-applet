#include "mainwindow.h"
#include "capturearea.h"
#include <QComboBox>
#include <QDebug>
#include <QKeyEvent>
#include "ui_mainwindow.h"

MainWindow::MainWindow() {
    this->w.setupUi(this);
    auto devices = this->conn.listDevices();

    // Connect before adding iterms so first addItem will call currentTextChanged
    // and select device.
    connect(this->w.devicesCombo, &QComboBox::currentTextChanged, [this](const QString& s) {
        this->conn.selectDevice(s.split(" - ").at(0));
    });

    connect(this->w.inputArea, &CaptureArea::keyPress, &this->conn, &KDEConnect::relayKeyPress);

    for (auto it = devices.constBegin(); it != devices.constEnd(); ++it) {
        this->w.devicesCombo->addItem(it.key() + " - " + it.value());
    }
}

MainWindow::~MainWindow() {
}
