#include "server.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QBuffer>
#include <QDir>
#include <QMovie>

Server::Server(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Server - Canlı Yayın");
       setFixedSize(640, 480);
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(640, 480);  // Sabit boyutla GIF'i ekrana sığdır




    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(imageLabel);
    setLayout(layout);

    QMovie *startMovie = new QMovie("/home/selinay/Downloads/gifi2.gif");
    if (startMovie->isValid()) {
        imageLabel->setMovie(startMovie);
        startMovie->setScaledSize(imageLabel->size());  // GIF'i tam ekran yapmak için QLabel'ın boyutuna ayarlıyoruz
        startMovie->start();
    } else {
        qDebug() << "Başlangıç GIF'i yüklenemedi!";
    }


    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::processPendingDatagrams);
}

Server::~Server() {
    udpSocket->close();
}

void Server::startServer() {
    if (!udpSocket->bind(QHostAddress::Any, 12345)) {
        qDebug() << "Server başlatılamadı!";
    } else {
        qDebug() << "Server başlatıldı, UDP paketi bekleniyor...";
    }
}

void Server::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(buffer.data(), buffer.size());

        if (buffer == "PHOTO_CAPTURED") {
            // Fotoğraf kaydetme sinyali alındı
            if (!lastReceivedImage.isNull()) {
                QString filename = QDir::homePath() + "/kamera/Pictures/server_photo.jpg";
                lastReceivedImage.save(filename);
                qDebug() << "Server'da fotoğraf kaydedildi: " << filename;
            }
        } else {
            // Gelen görüntüyü göster ve kaydet
            QImage img;
            img.loadFromData(buffer, "JPEG");

            if (!img.isNull()) {
                lastReceivedImage = img;
                imageLabel->setPixmap(QPixmap::fromImage(img.scaled(imageLabel->size(), Qt::KeepAspectRatio)));
            }
        }
    }
}
