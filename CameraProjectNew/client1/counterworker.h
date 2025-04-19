// counterworker.h
#ifndef COUNTERWORKER_H
#define COUNTERWORKER_H

#include <QObject>
#include <QThread>

class CounterWorker : public QObject {
    Q_OBJECT

    public:
       explicit CounterWorker(QObject *parent = nullptr);
    public slots:

        void increment();  // Sayaç artırma fonksiyonu

    signals:
        void counterUpdated(int value); // Sayaç güncellendiğinde sinyal yay


    private:
        int counter;
};

#endif // COUNTERWORKER_H
