#include "mainwindow.h"

void mainwindow::createGui()
{
    makeMenuBar();
    makeCentralWidget();

    status = new QLabel;
    status->setText(tr("Welcome to qNewsPaper v") + VERSION);

    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);
    statusBar->addWidget(status);

    netmanager = new network;

    this->makeConnections();
}

void mainwindow::makeMenuBar()
{
    menuBar = new QMenuBar(this);

    menuFile = new QMenu(menuBar);
    menuTools = new QMenu(menuBar);
    menuHelp = new QMenu(menuBar);

    menuFile->setTitle(tr("File"));
    menuTools->setTitle(tr("Tools"));
    menuHelp->setTitle(tr("Help"));

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuTools);
    menuBar->addMenu(menuHelp);

    this->setMenuBar(menuBar);

    this->makeActions();

    menuFile->addAction(actionExit);
    menuTools->addAction(actionCurrentDate);
    menuTools->addAction(actionOpenNewspapersFolder);
    menuTools->addAction(actionOpenUnsupportedNewspapers);
    menuTools->addAction(actionPreferences);
    menuHelp->addAction(actionAboutqNewsPaper);
}

void mainwindow::makeActions()
{
    actionAboutqNewsPaper = new QAction(this);
    actionOpenNewspapersFolder = new QAction(this);
    actionOpenUnsupportedNewspapers = new QAction(this);
    actionPreferences = new QAction(this);
    actionExit = new QAction(this);
    actionCurrentDate = new QAction(this);

    actionAboutqNewsPaper->setText(tr("About"));
    actionOpenNewspapersFolder->setText(tr("Newspapers folder"));
    actionOpenUnsupportedNewspapers->setText(tr("Unsupported newspapers"));
    actionPreferences->setText(tr("Preferences"));
    actionExit->setText(tr("Exit"));
    actionCurrentDate->setText(tr("Current date"));
}

void mainwindow::makeCentralWidget()
{
    centralWidget = new QWidget(this);

    this->makeGroupBox();

    pushButtonDownload = new QPushButton(tr("Download"));
    pushButtonOpen = new QPushButton(tr("Open"));
    pushButtonOpenWeb = new QPushButton(tr("Open Website"));

    pushButtonDownload->setEnabled(false);
    pushButtonOpen->setEnabled(false);

    dateEditEdition = new QDateEdit;
    dateEditEdition->setDisplayFormat("dd/MM/yyyy");

    lineEditLink = new QLineEdit;
    lineEditLink->setText("http://qnewspaper.googlecode.com");
    lineEditLink->setReadOnly(true);

    progressBar = new QProgressBar;
    progressBar->setValue(0);

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->addWidget(groupBoxNewspapers);

    horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(pushButtonDownload);
    horizontalLayout->addWidget(pushButtonOpen);
    horizontalLayout->addWidget(pushButtonOpenWeb);
    horizontalLayout->addWidget(dateEditEdition);

    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(lineEditLink);
    verticalLayout->addWidget(progressBar);

    warning = new QLabel(tr("The list of newspapers is empty or does not exist, normally qNewsPaper\nmust download automatically the latest list available from the server,\nnote that the list is created or updated automatically when you start the\napplication, so make sure that you have a functional Internet connection\notherwise please configure your Internet connection and restart qNewsPaper.\nRemember that you can get free support at any time from the website\nThank you for using qNewsPaper."));

    this->setCentralWidget(centralWidget);
}

void mainwindow::makeGroupBox()
{
    groupBoxNewspapers = new QGroupBox(centralWidget);
    groupBoxNewspapers->setTitle(tr("Newspapers"));

    gridLayout = new QGridLayout;
    groupBoxNewspapers->setLayout(gridLayout);
}
