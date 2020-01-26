#pragma once

#include <qglobal.h>
#include <QWidget>
#include "ui_mainwindow.h"
#include "kdeconnect.h"

class MainWindow : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
    Q_DISABLE_MOVE(MainWindow)
public:
    explicit MainWindow();
    ~MainWindow() override;

private:
    Ui::MainWindow w;
    KDEConnect conn;
};
