#include "capturearea.h"
#include <qpalette.h>
#include <QPalette>
#include <QGuiApplication>
#include <QDebug>
#include <QClipboard>
#include <QKeyEvent>

namespace {
    auto stdText = QStringLiteral("Click and type text here (if Remote Keyboard is active) or drag files here to send them.");
    auto dropText = QStringLiteral("Release mouse to send files.");
}

CaptureArea::CaptureArea(QWidget* parent) : QLabel(parent) {
    this->setText(stdText);
    this->setAcceptDrops(true);
    this->setFocus();
}

void CaptureArea::dragEnterEvent(QDragEnterEvent *event) {
    this->setBackgroundRole(QPalette::Highlight);
    this->setText(dropText);
    event->acceptProposedAction();
}

void CaptureArea::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void CaptureArea::dragLeaveEvent(QDragLeaveEvent *event) {
    this->setBackgroundRole(QPalette::Background);
    this->setText(stdText);
    event->accept();
}

void CaptureArea::dropEvent(QDropEvent *event) {
    this->setBackgroundRole(QPalette::Background);
    this->setText(stdText);
    emit this->paste(event->mimeData());
}

void CaptureArea::keyPressEvent(QKeyEvent* ev) {
    if (ev->matches(QKeySequence::Paste)) {
        auto cb = QGuiApplication::clipboard();
        emit this->paste(cb->mimeData());
        return;
    }

    emit this->keyPress(ev);
}
