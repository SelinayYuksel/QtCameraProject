#include "client.h"
#include "timeworker.h"
#include "counterworker.h"
//#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBuffer>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QDir>

Client::Client(QWidget *parent) : QWidget(parent) {


    setWindowTitle("Client - Kamera Görüntüsü");
    setGeometry(0, 0, 640, 550);
    cameraLabel = new QLabel(this);
    cameraLabel->setFixedSize(640, 480);


    // *Arka planı kaldır, sadece kamera görüntüsü gözüksün*
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // Butonlar için bir container ve layout oluşturuyoruz
    buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet("background-color: white; ");
    QHBoxLayout *buttonLayout;
    buttonLayout = new QHBoxLayout(buttonContainer);  // Yatay yerleşim için QHBoxLayout kullanıyoruz
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    // Fotoğraf çekme butonu
    captureButton = new QPushButton("Fotoğraf Çek", buttonContainer);
    captureButton->setFixedSize(120, 40);
    captureButton->setStyleSheet("background-color: grey;");
    connect(captureButton, &QPushButton::clicked, this, &Client::capturePhoto);

    // Görüntü başlatma butonu
    startButton = new QPushButton("Görüntü Başlat", buttonContainer);
    startButton->setFixedSize(120, 40);
    startButton->setStyleSheet("background-color: grey;");
    connect(startButton, &QPushButton::clicked, this, &Client::startStreaming);

    // Görüntü durdurma butonu
    stopButton = new QPushButton("Görüntü Durdur", buttonContainer);
    stopButton->setFixedSize(120, 40);
    stopButton->setStyleSheet("background-color: grey;");
    connect(stopButton, &QPushButton::clicked, this, &Client::stopStreaming);

    // Video kaydetme butonu
    recordButton = new QPushButton("Kayıt Başlat", buttonContainer);
    recordButton->setFixedSize(120, 40);
    recordButton->setStyleSheet("background-color: grey;");
    connect(recordButton, &QPushButton::clicked, this, &Client::toggleRecording);  // Kayıt başlat/durdur

    // Kaydı Durdur Butonu
    stopRecordingButton = new QPushButton("Kaydı Durdur", buttonContainer);
    stopRecordingButton->setFixedSize(120, 40);
    stopRecordingButton->setStyleSheet("background-color: grey;");
    connect(stopRecordingButton, &QPushButton::clicked, this, &Client::stopRecording);

    // Yeni bir container ve layout ekliyoruz (buton ve label)
    this->bottomContainer = new QWidget(this);
    this->bottomContainer->setStyleSheet("background-color: white;");
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomContainer);  // Yatay yerleşim için QHBoxLayout kullanıyoruz
    //bottomLayout->setContentsMargins(10, 10, 10, 10);
    bottomLayout->setSpacing(0);


    this->counterButton = new QPushButton("Sayaç", this->bottomContainer);
    counterButton->setFixedSize(120, 40);
    counterButton->setStyleSheet("background-color: grey;");


    this->counterLabel = new QLabel("Sayaç: 0", this->bottomContainer);
    counterLabel->setStyleSheet("font-size: 16px; color: black; background-color: grey;");
    counterLabel->setAlignment(Qt::AlignCenter);
    //counterLabel = new QLabel("Sayaç: 0");


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(counterLabel);
    layout->addWidget(counterButton);




    counterWorker = new CounterWorker();
    counterThread = new QThread();
    counterWorker->moveToThread(counterThread);
    counterThread->start();


    //  Butona tıklanınca sayaç artırılsın
    connect(counterButton, &QPushButton::clicked, counterWorker, &CounterWorker::increment, Qt::QueuedConnection);

    connect(counterWorker, &CounterWorker::counterUpdated, this, &Client::updateCounter);

    buttonLayout->addWidget(captureButton);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(recordButton);
    buttonLayout->addWidget(stopRecordingButton);
    buttonLayout->setSpacing(0);

    bottomLayout->addWidget(counterButton);
    bottomLayout->addWidget(counterLabel);

    // Layout'u kameranın altına ekliyoruz
    buttonContainer->setLayout(buttonLayout);

    // Anlık saat ve tarih bilgisi için QLabel
    timeLabel = new QLabel(this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("background-color: white; color: black; font-size: 20px;");
    timeLabel->setFixedHeight(40);

    // Kamera görüntüsünün üst kısmına logoları yerleştir
    this->logocontainer = new QWidget(this);
    int cameraHeight = cameraLabel->geometry().height();
    logocontainer->setGeometry(0, cameraHeight + -300, 640, 100);


    QHBoxLayout *logoLayout = new QHBoxLayout(logocontainer);
    logoLayout->setContentsMargins(0, 0, 0, 0);
    logoLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);  // Yatayda ortalamak ve üst kısmı hizalamak

    // Sol logo
    leftLogo = new QLabel(this);
    QPixmap leftPixmap("/home/selinay/Downloads/logo.png");
    leftLogo->setPixmap(leftPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    leftLogo->setStyleSheet("background: transparent;");

    // Sağ logo
    rightLogo = new QLabel(this);
    QPixmap rightPixmap("/home/selinay/Downloads/logo.png");
    rightLogo->setPixmap(rightPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    rightLogo->setStyleSheet("background: transparent;");

    // Logoları hizala
    logoLayout->addWidget(leftLogo, 0, Qt::AlignLeft);
    logoLayout->addStretch();  // Ortada boşluk bırak
    logoLayout->addWidget(rightLogo, 0, Qt::AlignRight);

    // Logo container'ı ana layout'a ekliyoruz
    logocontainer->setLayout(logoLayout);


    // Ana layout'u oluşturuyoruz
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(cameraLabel);
    mainLayout->addWidget(buttonContainer);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(bottomContainer);
    setLayout(mainLayout);

    udpSocket = new QUdpSocket(this);
    serverAddress = QHostAddress("192.168.133.127"); // Server IP
    serverPort = 12345; // Server UDP port

    capture.open(0);
    if (!capture.isOpened()) {
        qDebug() << "Kamera açılamadı!";
        return;
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Client::sendFrame);

    // Saat ve tarihi güncellemek için thread başlatıyoruz
    timeThread = new QThread(this);
    timeWorker = new TimeWorker();  // Saat güncelleme işçisi
    timeWorker->moveToThread(timeThread);  // Worker'ı thread'e taşıyoruz

    connect(timeThread, &QThread::started, timeWorker, &TimeWorker::startWorking);

    connect(timeWorker, &TimeWorker::timeUpdated, this, &Client::updateTime);  // Saat güncellendikçe UI'yı güncelliyoruz
     timeThread->start();
}

Client::~Client() {
    if (capture.isOpened()) {
        capture.release();
    }
    if (timer) {
        timer->stop();
    }

    delete cameraLabel;
    delete counterLabel;
    delete counterButton;
    delete captureButton;
    delete startButton;
    delete stopButton;
    delete recordButton;
    delete stopRecordingButton;
    delete buttonContainer;
    delete bottomContainer;
    delete udpSocket;
    timeThread->quit();
    timeThread->wait();
    delete timeThread;
    counterThread->quit();
    counterThread->wait();
    delete counterThread;
    delete timeWorker;
    delete counterWorker;

}
void Client::sendFrame() {
    cv::Mat frame;
    capture >> frame;
    if (frame.empty()) return;

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // *Görüntüyü küçült (örneğin, 320x240 olarak ayarla)*
    QImage scaledImg = img.scaled(320, 240, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


    // QLabel üzerinden canlı görüntüyü göster
    cameraLabel->setPixmap(QPixmap::fromImage(img.scaled(cameraLabel->size(), Qt::KeepAspectRatio)));

    QByteArray buffer;
    QBuffer qBuffer(&buffer);
    qBuffer.open(QIODevice::WriteOnly);
    img.scaled(640, 480, Qt::KeepAspectRatio).save(&qBuffer, "JPEG");

    udpSocket->writeDatagram(buffer, serverAddress, serverPort);
    // Eğer video kaydımız aktifse, görüntüyü kaydet
    if (isRecording) {
        Videowriter.write(frame);
    }
}

void Client::startStreaming() {
    if (!timer->isActive()) {
        timer->start(50);  // 20 FPS
        qDebug() << "Görüntü akışı başlatıldı.";
    }
}

void Client::stopStreaming() {
    if (timer->isActive()) {
        timer->stop();
        qDebug() << "Görüntü akışı durduruldu.";
    }
}

void Client::capturePhoto() {
    cv::Mat frame;
    capture >> frame;
    if (frame.empty()) return;

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // Fotoğrafı kaydedelim
    QString folderPath = QDir::homePath() + "/kamera";
    QDir dir(folderPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString filename = folderPath + "/client_photo.jpg";
    img.save(filename);
    qDebug() << "Fotoğraf kaydedildi: " << filename;
}

void Client::toggleRecording() {
    // Video kaydının yapılacağı klasör yolu
    QString folderPath = QDir::homePath() + "/kamera";
    QDir dir(folderPath);

    // Eğer klasör yoksa oluşturuyoruz
    if (!dir.exists()) {
        bool created = dir.mkpath(".");  // "." ile mevcut yolu oluşturuyoruz
        if (!created) {
            qDebug() << "Klasör oluşturulamadı!";
            return;
        }
    }

    // Kaydedilecek video dosya yolu
    QString savePath = folderPath + "/video_record.avi";  // Kameranın kaydedeceği video dosyası

    if (isRecording) {
        // Kaydı durdur
        Videowriter.release();  // VideoWriter'ı serbest bırak
        isRecording = false;
        qDebug() << "Video kaydı durduruldu.";
    } else {
        // Kaydı başlat
        int frameWidth = capture.get(cv::CAP_PROP_FRAME_WIDTH);  // Kamera genişliği
        int frameHeight = capture.get(cv::CAP_PROP_FRAME_HEIGHT); // Kamera yüksekliği
        Videowriter.open(savePath.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(frameWidth, frameHeight), true);

        if (!Videowriter.isOpened()) {
            qDebug() << "Video kaydı başlatılamadı!";
            return;
        }

        isRecording = true;
        qDebug() << "Video kaydı başlatıldı.";
    }
}

void Client::stopRecording() {
    if (isRecording) {
        // Video kaydını durdur
        Videowriter.release();
        isRecording = false;
        qDebug() << "Video kaydı durduruldu.";
    }
}



// Saat güncellenmesi durumu
void Client::updateTime(const QString &timeString) {
    timeLabel->setText(timeString);
}

void Client::onTimeUpdated(const QString &timeAndDate) {
    timeLabel->setText(timeAndDate);
}

void Client::incrementCounter() {
    counterWorker->increment();
}


void Client::updateCounter(int value) {
    counterLabel->setText("Sayaç: " + QString::number(value));
}


//  Sayaç güncellenince label'ı güncelle
void Client::onCounterUpdated(int value) {
    counterLabel->setText(QString::number(value));
}
//  Butona basılınca sayaç artırma fonksiyonu
void Client::onCounterButtonClicked() {
  QMetaObject::invokeMethod(counterWorker, "increment", Qt::QueuedConnection);
}

