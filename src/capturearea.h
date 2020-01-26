#pragma once

#include <qglobal.h>
#include <qobjectdefs.h>
#include <QLabel>

class CaptureArea : public QLabel {
    Q_OBJECT
    Q_DISABLE_COPY(CaptureArea)
    Q_DISABLE_MOVE(CaptureArea)
public:
    CaptureArea(QWidget* parent = nullptr);
signals:
    void keyPress(QKeyEvent* ev);

protected:
    void keyPressEvent(QKeyEvent* ev) override;
};
