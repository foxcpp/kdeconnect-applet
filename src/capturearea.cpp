#include "capturearea.h"
#include <qdebug.h>

CaptureArea::CaptureArea(QWidget* parent) : QLabel(parent) {
}

void CaptureArea::keyPressEvent(QKeyEvent *ev) {
    emit this->keyPress(ev);
}
