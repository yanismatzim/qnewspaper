#include "network.h"
#include "math.h"

network::network()
{
    currentState = Idle;

    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.6) Gecko/20100625 Firefox/3.6.6");
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language", "fr,fr-fr;q=0.8,en-us,image/png,image/*;q=0.5,en;q=0.3");
    request.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
    request.setRawHeader("Keep-Alivet", "115");
    request.setRawHeader("Connection", "keep-alive");

    manager = new QNetworkAccessManager(this);
}

void network::download(QString url)
{
    currentState = Download;

    request.setUrl(url);
    request.setRawHeader("Host", request.url().host().toUtf8());

    reply = manager->get(request);

    time.start();

    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgress(qint64,qint64)));
}

void network::updateProgress()
{
    double speed = m_bytesReceived * 1000.0 / time.elapsed();
    double timeRemain = ((double)(m_bytesTotal - m_bytesReceived)) / speed;

    QString timeRe = "sec";
    if (timeRemain > 60)
    {
        timeRemain /= 60;
        timeRe = "min";
    }

    timeRemain = floor(timeRemain);

    if(timeRemain == 0)
        timeRemain = 1;

    QString ALL_INFO;
    QString restant;

    if (m_bytesTotal != 0)
    {
        if(timeRemain > 3600)
        {
            restant = tr("- unknown time remaining");
        }
        else
        {
        restant = tr("- %2 %3 remaining")
                .arg(timeRemain)
                .arg(timeRe);
        }
        ALL_INFO = QString(tr("Downloading at %1/sec %2"))
                .arg(dataString((int)speed))
                .arg(restant);
    }
    emit updateStatusBar(ALL_INFO);
}

void network::grabLink(qRadioButton *newspaper)
{
    currentState = Grab;

    request.setUrl(newspaper->getUrl());
    request.setRawHeader("Host", request.url().host().toUtf8());

    reply = manager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(grabFinished()));

    emit updateStatusBar(tr("Please wait while grabbing the latest link"));
}

void network::grabFinished()
{
    qDebug()<<reply->readAll();
    if(reply->error())
        emit updateStatusBar(reply->errorString());
    else
    {
        QFile file(QDir::tempPath()+"/qnp");
        if (file.open(QIODevice::ReadWrite))
        {
            file.write(reply->readAll());
            emit parse(&file);
        }
        else
            emit updateStatusBar(tr("Error : could not access to temporary files"));
    }

    reply->deleteLater();
    currentState = Idle;
}

QString network::dataString(int bytes) const
{
    QString unit;
    if (bytes < 1024) {
        unit = "bytes";
    } else if (bytes < 1024*1024) {
        bytes /= 1024;
        unit = "kB";
    } else {
        bytes /= 1024*1024;
        unit = "MB";
    }
    return QString(QLatin1String("%1 %2")).arg(bytes).arg(unit);
}

void network::head(QString url)
{
    currentState = Head;

    request.setUrl(url);
    request.setRawHeader("Host", request.url().host().toUtf8());
    reply = manager->head(request);

    connect(reply, SIGNAL(finished()), this, SLOT(headFinished()));

    emit updateStatusBar(tr("Checking for NewsPaper..."));
}

void network::headFinished()
{
    if(reply->error())
    {
        if(reply->error() == 203)
            emit updateStatusBar(tr("Newspaper not found"));
        else
            emit updateStatusBar(reply->errorString());
    }
    else
    {
        emit enableDownload();
        emit updateStatusBar(tr("Newspaper found click the download button !"));
    }
    reply->deleteLater();

    currentState = Idle;
}

int network::getState()
{
    return currentState;
}

void network::updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit sendDownloadProgress(bytesReceived, bytesTotal);
    m_bytesReceived  = bytesReceived;
    m_bytesTotal = bytesTotal;
    updateProgress();
}

void network::downloadFinished()
{
    if(reply->error())
    {
        emit updateStatusBar(reply->errorString());
        emit deleteFile();
    }
    else
    {
        emit writeData(reply);
        emit updateStatusBar(tr("Download finished"));
    }

    emit resetDownloadGui();

    reply->deleteLater();
    currentState = Idle;
}

void network::checkForUpdate()
{
    managerUpdate = new QNetworkAccessManager(this);
    replyUpdate = managerUpdate->get(QNetworkRequest(QUrl("http://qnp.byethost7.com/index.xml")));

    connect(replyUpdate, SIGNAL(finished()), this, SLOT(checkForUpdateFinished()));
}

void network::checkForUpdateFinished()
{
    emit updateInformation(replyUpdate->readAll());
    managerUpdate->deleteLater();
    replyUpdate->deleteLater();
}

void network::abort()
{
    reply->abort();
}

network::~network()
{
    if(currentState)
        reply->abort();

    reply->deleteLater();
}
