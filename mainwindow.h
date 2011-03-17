#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QDialog>
#include <QDesktopServices>
#include <QDomDocument>
#include <QDateEdit>
#include <QDir>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QCoreApplication>
#include <QStatusBar>
#include <QSettings>
#include <QNetworkReply>
#include <QList>
#include <QUrl>
#include <QFileDialog>
#include <QFormLayout>
#include <QCloseEvent>

#include "newspaper.h"
#include "network.h"

#include <QDebug>

static QString VERSION("1.31");

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);
    void canOpen(int ID);
    void createGui();
    void makeMenuBar();
    void makeActions();
    void makeCentralWidget();
    void makeGroupBox();
    void loadNewspapersList();
    void loadSettings();
    void makeConnections();
    void closeEvent(QCloseEvent *event);
    ~mainwindow();

public slots:
    void enableDownload();
    void updateStatusBar(QString msg);
    void updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal);
    void resetDownloadGui();
    void selectDir();
    void parse(QFile *html);
    void writeData(QNetworkReply *reply);
    void installUpdate(QNetworkReply *reply);
    void updateFail();
    void deleteFile();
    void preferencesDialog();
    void loadNewspapersPreferencesList(QGridLayout *showHideGridLayout);
    void checkForUpdate(QString updateInfo);
    QDate getDate();

private slots:
    void languageChanged(int index);
    void openNewspapersFolder();
    void download();
    void currentDate();
    void XML_updater();
    void openFile();
    void openWeb();
    void dateChanged();
    void newspaperSelected();
    void unsupportedMessageBox();
    void about();

private:
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuHelp;

    QAction *actionAboutqNewsPaper;
    QAction *actionOpenNewspapersFolder;
    QAction *actionOpenUnsupportedNewspapers;
    QAction *actionPreferences;
    QAction *actionExit;
    QAction *actionCurrentDate;

    QWidget *centralWidget;
    QLabel *warning;
    QGroupBox *groupBoxNewspapers;
    QPushButton *pushButtonDownload;
    QPushButton *pushButtonOpen;
    QPushButton *pushButtonOpenWeb;
    QDateEdit *dateEditEdition;
    QLineEdit *lineEditLink;
    QProgressBar *progressBar;

    QLabel *status;
    QStatusBar *statusBar;

    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

    QList<qRadioButton *> newspaperList;
    QLabel unsupported;
    QString unsupportedLinks;
    int currentNewspaperID;

    network *netmanager;

    QDir newspapersFolder;
    QDir workingDir;
    QFileInfo open;
    QFile file;

    QSettings settings;

    //Config Dialog
    QLineEdit *lineEditCurrentFolder;
    QLabel *labelLanguageNote;
    QList<QCheckBox *> newspaperCheckBoxs;
};

#endif // MAINWINDOW_H
