#ifndef CLIENT_H
#define CLIENT_H
#include "counterworker.h"
#include "timeworker.h"
#include "qboxlayout.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QTimer>
#include <QThread>
#include <QUdpSocket>
#include <opencv2/opencv.hpp>
#include <QDir>
#include <QStackedWidget>



class Client : public QWidget {
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();



public slots:
    void startStreaming();
    void stopStreaming();
    void capturePhoto();
    void sendFrame();
    void toggleRecording();
    void stopRecording();
    void updateTime(const QString &timeAndDate);
    void incrementCounter();  // Sayacı artırma işlevi
    void onTimeUpdated(const QString &timeAndDate) ;
    void updateCounter(int newCount);
    void onCounterUpdated(int value);  // Sayaç güncellendiğinde çalışacak
    void onCounterButtonClicked();


private:
    QIcon rightIcon;
    QIcon leftIcon;
    QLabel *cameraLabel;
    QLabel *timeLabel;
    QLabel *counterLabel;
    QLabel *rightLogo;
    QLabel *leftLogo;
    QThread *timeThread;
    QThread *workerThread;
    QThread *counterThread;
    CounterWorker *counterWorker;
    TimeWorker *timeWorker;
    QPushButton *captureButton;
    QPushButton *stopButton;
    QPushButton *startButton;
    QPushButton *recordButton;
    QPushButton *stopRecordingButton;
    QPushButton *counterButton;
    QWidget *buttonContainer;
    QWidget *bottomContainer;
    QWidget *logocontainer;
    QStackedWidget *stackedWidget;
    QVBoxLayout *buttonLayout;
    QHBoxLayout *bottomLayout;
    QUdpSocket *udpSocket;
    cv::VideoCapture capture;
    cv:: VideoWriter Videowriter;
    bool isRecording =false;
    QTimer *timer;
    QHostAddress serverAddress;
    quint16 serverPort;
    int counter =0;
    QPixmap rightPixmap;
    QPixmap leftPixmap;

signals:
    void counterUpdated(int newCounter);
};

#endif // CLIENT_H
