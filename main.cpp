#include <QtGui/QApplication>
#include <QTranslator>
#include <QSettings>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("qNewsPaper");
    QCoreApplication::setOrganizationDomain("qnewspaper.googlecode.com");
    QCoreApplication::setApplicationName("Hakim-3i & Zino");

    QTranslator translator;
    QString language;

    QSettings settings("qNewsPaper", "Hakim-3i & Zino");

    if(!settings.contains("language"))
    {
        language = QLocale::system().name();
        settings.setValue("language", language);
        settings.value("XML_VERSION").setValue(0);
    }
    else
        language = settings.value("language").toString();

    translator.load("qnewspaper_" + language);
    a.installTranslator(&translator);

    mainwindow w;
    w.show();
    return a.exec();
}
