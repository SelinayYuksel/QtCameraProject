// counterworker.cpp
#include "counterworker.h"
#include <QTimer>

CounterWorker::CounterWorker(QObject *parent) : QObject(parent), counter(0) {

}

void CounterWorker::increment() {
    counter++;
    emit counterUpdated(counter);  // Sayaç güncellendiğinde sinyal yay
}

