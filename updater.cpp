#include "mainwindow.h"

void mainwindow::checkForUpdate(QString updateInfo)
{
    QDomDocument DOM;
    DOM.setContent(updateInfo);

    QDomNode Node;
    Node = DOM.documentElement().firstChild();

    while(!Node.isNull())
    {
        QDomElement Element = Node.toElement();
        if(Element.tagName() == "programe")
        {
            if(Element.attribute("version") > VERSION)
            {
                int ret = QMessageBox::information(this, "qNewsPaper", tr("There are an update available for qNewsPaper\n")+ tr("you are running the version ") +
                                         VERSION + tr(" and the latest one is ") + Element.attribute("version") +
                                         tr("\nWant to update the application ?"), QMessageBox::Yes | QMessageBox::No);
                switch(ret)
                {
                case QMessageBox::Yes:
                    #ifdef Q_OS_WIN32
                        lineEditLink->setText(tr("Downloading qNewsPaper update please wait !"));
                        file.setFileName("qNewsPaper-installer.exe");
                        if(file.open(QIODevice::WriteOnly))
                        {
                            pushButtonDownload->setText("Stop");
                            pushButtonDownload->setEnabled(true);
                            pushButtonDownload->disconnect();
                            netmanager->disconnect(SIGNAL(deleteFile()));
                            netmanager->disconnect(SIGNAL(writeData(QNetworkReply*)));
                            connect(netmanager, SIGNAL(deleteFile()), this, SLOT(updateFail()));
                            connect(netmanager, SIGNAL(writeData(QNetworkReply*)), this, SLOT(installUpdate(QNetworkReply*)));
                            connect(pushButtonDownload, SIGNAL(clicked()), netmanager, SLOT(abort()));
                            netmanager->download(Element.attribute("download_win"));
                            return;
                        }
                        QMessageBox::critical(this, tr("Error"), tr("Unable to write on the file ") + "qNewsPaper-installer.exe");
                    #else
                        QDesktopServices::openUrl(QUrl("http://code.google.com/p/qnewspaper/downloads/list");
                    #endif
                    break;
                }
            }
        }
        else if(Element.tagName() == "xmlfile")
        {
            file.setFileName("newspapers.xml");
            if(file.open(QIODevice::ReadWrite))
            {
                if(Element.attribute("version").toDouble() != settings.value("XML_VERSION").toDouble() || file.readAll().isEmpty())
                {
                    if(file.readAll().isEmpty())
                        QMessageBox::information(this, "qNewsPaper", tr("The list of newspapers has been successfully downloaded from the server, remember that your support are very welcome <a href=\"http://qnewspaper.googlecode.com\">website</a>, now you can start using qNewsPaper. Happy reading :)"));
                    else
                        QMessageBox::information(this, "qNewsPaper", tr("qNewsPaper has updated automatically the list of newspapers what's new :\n") + Element.attribute("new") +
                                             tr("\nNote : your preferences regarding the list of newspapers will be overwritten the menu tools -> preferences allow you to reconfigure the list."));
                    file.remove();
                    file.open(QIODevice::ReadWrite);
                    file.write(Element.attribute("file").toUtf8());
                    settings.setValue("XML_VERSION", Element.attribute("version"));
                }
            }
            else
                QMessageBox::critical(this, tr("Error"), tr("Unable to write on the file ") + "newspapers.xml");
            file.close();
        }
        Node = Node.nextSibling();
    }
    if(updateInfo.isEmpty())
        status->setText(tr("Failed to check for updates"));
    this->loadNewspapersList();
}

void mainwindow::installUpdate(QNetworkReply *reply)
{
    file.write(reply->readAll());
    file.close();

    QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/qNewsPaper-installer.exe", QUrl::TolerantMode));
    this->close();
}

void mainwindow::updateFail()
{
    file.remove();
    file.close();

    netmanager->disconnect(SIGNAL(deleteFile()));
    netmanager->disconnect(SIGNAL(writeData(QNetworkReply*)));
    connect(netmanager, SIGNAL(deleteFile()), this, SLOT(deleteFile()));
    connect(netmanager, SIGNAL(writeData(QNetworkReply*)), this, SLOT(writeData(QNetworkReply*)));
}
