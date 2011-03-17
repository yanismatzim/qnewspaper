#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QTime>
#include <QDir>

#include <QDebug>

#include "newspaper.h"

class network : public QObject
{
    Q_OBJECT

public:
    network();
    void download(QString url);
    void grabLink(qRadioButton *newspaper);
    void head(QString url);
    void updateProgress();
    void checkForUpdate();
    QString dataString(int bytes) const;
    int getState();
    ~network();

signals:
    void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void enableDownload();
    void replyDone(QNetworkReply *reply);
    void updateStatusBar(QString msg);
    void updateInformation(QString updateInfo);
    void deleteFile();
    void writeData(QNetworkReply *reply);
    void parse(QFile *file);
    void resetDownloadGui();

public slots:
    void abort();

private slots:
    void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void headFinished();
    void grabFinished();
    void downloadFinished();
    void checkForUpdateFinished();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QNetworkReply* reply;
    QNetworkAccessManager *managerUpdate;
    QNetworkReply* replyUpdate;
    quint64 m_bytesReceived;
    quint64 m_bytesTotal;
    QTime time;

    enum STATE{
        Idle = 0,
        Head,
        Grab,
        Download
    };

    STATE currentState;
};

#endif // NETWORK_H
