#include <QApplication>
#include <QWidget>
#include "client.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Client client;
    client.show();
    client.startStreaming();
    TimeWorker timeWorker;
    CounterWorker counterWorker;

    timeWorker.startWorking();


    return app.exec();
}
