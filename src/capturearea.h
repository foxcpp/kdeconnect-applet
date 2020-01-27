#pragma once

#include <QObject>
#include <QMimeData>
#include <QLabel>

class CaptureArea : public QLabel {
    Q_OBJECT
    Q_DISABLE_COPY(CaptureArea)
    Q_DISABLE_MOVE(CaptureArea)
public:
    CaptureArea(QWidget* parent = nullptr);
signals:
    void keyPress(QKeyEvent* ev);
    void paste(const QMimeData* d);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent* ev) override;
};
