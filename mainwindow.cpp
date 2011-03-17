#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    createGui();
    loadSettings();
}

void mainwindow::loadNewspapersList()
{
    for(int i = 0; i < newspaperList.size(); i++)
        newspaperList[i]->deleteLater();

    newspaperList.clear();
    unsupportedLinks.clear();

    QDomDocument DOM;

    #ifdef Q_OS_WIN32
        file.setFileName(QCoreApplication::applicationDirPath() + "/newspapers.xml");
    #else
        file.setFileName("/usr/share/qnewspaper/newspapers.xml");
    #endif

    file.open(QIODevice::ReadOnly);
    DOM.setContent(file.readAll());
    file.close();

    int i = 0;
    int r = 0;
    int c = 0;

    QDomNode Node;
    Node = DOM.documentElement().firstChild();

    while(!Node.isNull())
    {
        QDomElement Element = Node.toElement();

        if(Element.tagName() == "newspaper")
        {
            if(Element.attribute("hidden") == "false")
            {
                qRadioButton *newspaper = new qRadioButton(Element.attribute("name"), Element.attribute("website"),
                                                           Element.attribute("filelocation"), Element.attribute("today"),
                                                           Element.attribute("extension"), Element.attribute("type"),
                                                           Element.attribute("regexp"), i);
                gridLayout->addWidget(newspaper, r, c);
                connect(newspaper, SIGNAL(clicked()), this, SLOT(newspaperSelected()));
                connect(newspaper, SIGNAL(getDate()), this, SLOT(getDate()));
                newspaperList << newspaper;

                r++;
                i++;
            }
        }
        else if(Element.tagName() == "link")
            unsupportedLinks += "<a href=\"http://" + Element.attribute("website") +"\">" + Element.attribute("name") + "</a>, ";

        Node = Node.nextSibling();
        if(r == 10)
        {
            r = 0;
            c++;
        }
    }
    if(DOM.toString().isEmpty())
    {
        gridLayout->addWidget(warning);
        actionOpenUnsupportedNewspapers->setEnabled(false);
    }
    else
        actionOpenUnsupportedNewspapers->setEnabled(true);
}

void mainwindow::makeConnections()
{
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionOpenNewspapersFolder, SIGNAL(triggered()), this, SLOT(openNewspapersFolder()));
    connect(actionOpenUnsupportedNewspapers, SIGNAL(triggered()), this, SLOT(unsupportedMessageBox()));
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(preferencesDialog()));
    connect(actionCurrentDate, SIGNAL(triggered()), this, SLOT(currentDate()));
    connect(actionAboutqNewsPaper, SIGNAL(triggered()), this, SLOT(about()));

    connect(pushButtonDownload, SIGNAL(clicked()), this, SLOT(download()));
    connect(pushButtonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(pushButtonOpenWeb, SIGNAL(clicked()), this, SLOT(openWeb()));

    this->currentDate();
    connect(dateEditEdition, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged()));

    connect(netmanager, SIGNAL(updateStatusBar(QString)), this, SLOT(updateStatusBar(QString)));
    connect(netmanager, SIGNAL(enableDownload()), this, SLOT(enableDownload()));
    connect(netmanager, SIGNAL(parse(QFile*)), this, SLOT(parse(QFile*)));
    connect(netmanager, SIGNAL(sendDownloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgressBar(qint64,qint64)));
    connect(netmanager, SIGNAL(writeData(QNetworkReply*)), this, SLOT(writeData(QNetworkReply*)));
    connect(netmanager, SIGNAL(deleteFile()), this, SLOT(deleteFile()));
    connect(netmanager, SIGNAL(resetDownloadGui()), this, SLOT(resetDownloadGui()));
    connect(netmanager, SIGNAL(updateInformation(QString)), this, SLOT(checkForUpdate(QString)));

    connect(&unsupported, SIGNAL(linkActivated(QString)), this, SLOT(unsupportedMessageBox()));
}

void mainwindow::unsupportedMessageBox()
{
    QMessageBox::information(this, "Newspaper", "<b>Note:</b> " + tr("This is a list of some newspapers that qNewsPaper does not support yet, but you can always click on the links to visit the official website of the publisher :") + "<br>" + unsupportedLinks);
}

void mainwindow::openNewspapersFolder()
{
    QDesktopServices::openUrl(QUrl("file:///" + newspapersFolder.path(), QUrl::TolerantMode));
}

void mainwindow::currentDate()
{
    dateEditEdition->setDate(QDate::currentDate());
}

void mainwindow::about()
{
    QMessageBox::information(this, "About",tr("qNewsPaper Algerian newspapers downloader") + "<br><b>Version :</b> " + VERSION + "<br><b>" + tr("Newspapers list :") + "</b> " + settings.value("XML_VERSION").toString() + "<br><b>Website :</b> <a href=\"http://code.google.com/p/qnewspaper/\">http://code.google.com/p/qnewspaper/</a>");
}

void mainwindow::openWeb()
{
    if(currentNewspaperID == -1)
        QDesktopServices::openUrl(QUrl("http://qnewspaper.googlecode.com"));
    else
        QDesktopServices::openUrl("http://" + QUrl(newspaperList[currentNewspaperID]->getUrl()).host());
}

void mainwindow::dateChanged()
{
    if(currentNewspaperID == -1)
        return;

    if(netmanager->getState() > 0 && netmanager->getState() < 3)
        netmanager->abort();

    if(newspaperList[currentNewspaperID]->isChecked())
    {
        lineEditLink->setText(newspaperList[currentNewspaperID]->getUrl());
        newspaperList[currentNewspaperID]->click();
    }
}

void mainwindow::newspaperSelected()
{
    qRadioButton *newspaper = qobject_cast<qRadioButton *>(sender());

    this->canOpen(newspaper->getID());

    if(netmanager->getState() == 3)
        return;

    pushButtonDownload->setEnabled(false);

    lineEditLink->setText(newspaper->getUrl());

    currentNewspaperID = newspaper->getID();

    if(netmanager->getState())
        netmanager->abort();

    switch(newspaper->getType())
    {
    case 0:
        dateEditEdition->setEnabled(true);
        netmanager->head(newspaper->getUrl());
        break;
    case 1:
        dateEditEdition->setEnabled(false);
        dateEditEdition->setDate(QDate::currentDate());
        //dateEditEdition->setDate(QDate::currentDate()); n'aide pas !
        netmanager->grabLink(newspaper);
        /*We have to parse the date from the link*/
        break;
    case 2:
        dateEditEdition->setEnabled(false);
        currentDate();
        enableDownload();
        status->setText(tr("Click to download the latest edition"));
        break;
    case 3:
        dateEditEdition->setEnabled(true);
        if(dateEditEdition->date() == QDate::currentDate())
        {
            lineEditLink->setText(newspaper->getTodayUrl());
            status->setText(tr("Click to download the latest edition"));
            enableDownload();
        }
        else
        {
            netmanager->head(newspaper->getUrl());
        }
    }
}

void mainwindow::canOpen(int ID)
{
    #ifdef Q_OS_WIN32
        open.setFile(newspapersFolder.path() + dateEditEdition->date().toString("/dd MM yyyy//") + newspaperList[ID]->getFilename());
    #else
        open.setFile(newspapersFolder.path() + dateEditEdition->date().toString("/dd_MM_yyyy//") + newspaperList[ID]->getFilename());
    #endif
    pushButtonOpen->setEnabled(open.exists()&& open.size());
}

void mainwindow::openFile()
{
    QDesktopServices::openUrl(QUrl("file:///" + open.filePath()));
}

void mainwindow::parse(QFile *html)
{
    QString line;
    QRegExp rx(newspaperList[currentNewspaperID]->getRx());
    QString captured;
    QTextStream stream(html);
    stream.seek(0);

    do{
        line = stream.readLine();
        rx.indexIn(line);
        captured = rx.cap(1);
    }while(captured.isNull() && !line.isNull());

    if(captured.isEmpty())
        status->setText(tr("An error occured, please try again later !"));
    else
    {
        QString str = lineEditLink->text() + captured;
        str.remove("amp;");
        lineEditLink->setText(str);
        status->setText(tr("Newspaper found click the download button !"));
        enableDownload();
    }

    html->close();
    html->remove();
}

void mainwindow::download()
{
    #ifdef Q_OS_WIN32
        newspapersFolder.mkdir(dateEditEdition->date().toString("dd MM yyyy"));
        workingDir.setCurrent(newspapersFolder.path() + dateEditEdition->date().toString("/dd MM yyyy"));
    #else
        newspapersFolder.mkdir(dateEditEdition->date().toString("dd_MM_yyyy"));
        workingDir.setCurrent(newspapersFolder.path() + dateEditEdition->date().toString("/dd_MM_yyyy"));
    #endif

    QString filename = newspaperList[currentNewspaperID]->getFilename();

    #ifndef Q_OS_WIN32
        filename.replace(" ", "_");
        filename.replace("'", "_");
    #endif

    if (QFile::exists(filename))
    {
        if (QMessageBox::question(this, "qNewsPaper", tr("There already exists a file called ") + filename +
            tr(" in the current directory. Overwrite it ?"),
            QMessageBox::Yes | QMessageBox::No)
            == QMessageBox::No)
                return;

        if(!QFile::remove(filename))
        {
            QMessageBox::critical(this, tr("Error"), tr("Unable to remove ") + filename);
            return;
        }
        pushButtonOpen->setEnabled(false);
    }

    file.setFileName(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"), tr("Unable to write on the file ") + filename);
        return;
    }
    
    netmanager->download(lineEditLink->text());

    pushButtonDownload->setText("Stop");
    pushButtonDownload->disconnect();
    connect(pushButtonDownload, SIGNAL(clicked()), netmanager, SLOT(abort()));
}

void mainwindow::resetDownloadGui()
{
    pushButtonDownload->setText("Download");
    pushButtonDownload->disconnect();
    connect(pushButtonDownload, SIGNAL(clicked()), this, SLOT(download()));

    pushButtonDownload->setEnabled(false);

    progressBar->setValue(0);
}

void mainwindow::deleteFile()
{
    file.remove();
    file.close();
    QString lastDir = workingDir.absolutePath();
    workingDir.setCurrent(newspapersFolder.path());
    newspapersFolder.rmdir(lastDir);
}

void mainwindow::writeData(QNetworkReply *reply)
{
    file.write(reply->readAll());
    file.close();

    canOpen(currentNewspaperID);
}

QDate mainwindow::getDate()
{
    return dateEditEdition->date();
}

void mainwindow::updateStatusBar(QString msg)
{
    status->setText(msg);
}

void mainwindow::updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal)
{
    progressBar->setMaximum(bytesTotal);
    progressBar->setValue(bytesReceived);
}

void mainwindow::enableDownload()
{
    pushButtonDownload->setEnabled(true);
}

void mainwindow::closeEvent(QCloseEvent *event)
{
    if(netmanager->getState())
        netmanager->abort();

    event->accept();
}

mainwindow::~mainwindow()
{

}
