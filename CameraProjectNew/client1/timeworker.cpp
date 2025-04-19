#include "timeworker.h"
#include "qdatetime.h"
#include "qobject.h"
#include <QTimer>


TimeWorker::TimeWorker(QObject *parent) : QObject(parent) {

}


TimeWorker::~TimeWorker() {}

void TimeWorker::startWorking() {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        emit timeUpdated(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    });
    timer->start(1000);  // Her saniye bir güncelleme
}


