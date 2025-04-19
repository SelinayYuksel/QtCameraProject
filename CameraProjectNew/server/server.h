#ifndef SERVER_H
#define SERVER_H

#include "qmovie.h"
#include <QWidget>
#include <QUdpSocket>
#include <QLabel>

class Server : public QWidget {
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

    void startServer();

private slots:
    void processPendingDatagrams();

private:
    QLabel *imageLabel;
    QMovie startMovie;
    QPixmap startImage;
    QUdpSocket *udpSocket;
    QImage lastReceivedImage; // Gelen son görüntü

};

#endif // SERVER_H
