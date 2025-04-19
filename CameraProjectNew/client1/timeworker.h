#ifndef TIMEWORKER_H
#define TIMEWORKER_H
#include "qobject.h"


class TimeWorker : public QObject {
    Q_OBJECT

public:
    explicit TimeWorker(QObject *parent = nullptr);
    ~TimeWorker();

signals:
    void timeUpdated(const QString &timeAndDate);  // Saat ve tarih güncellemesi
public slots:
    void startWorking();


private:

    TimeWorker *timeWorker;
};
#endif // TIMEWORKER_H
