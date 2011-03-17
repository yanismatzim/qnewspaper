#include "mainwindow.h"

void mainwindow::loadSettings()
{
    if(!settings.contains("newspapersFolder"))
    {
        newspapersFolder.setPath(QDir::homePath());
        newspapersFolder.mkdir("Newspapers");
        settings.setValue("newspapersFolder", QDir::homePath() + "/Newspapers");
    }

    newspapersFolder.setPath(settings.value("newspapersFolder").toString());
    currentNewspaperID = -1;

    this->setFixedSize(0, 0);

    netmanager->checkForUpdate();
}

void mainwindow::preferencesDialog()
{
    if(netmanager->getState() == 3)
    {
        QMessageBox::information(this, "qNewsPaper", tr("You must stop the download before accessing this menu"));
        return;
    }

    QDialog *preferencesDialog = new QDialog(this);
    preferencesDialog->setWindowTitle(tr("Preferences"));

    QTabWidget *preferencesTabs = new QTabWidget();
    preferencesTabs->setTabPosition(QTabWidget::West);

    QWidget *optionsTab = new QWidget;
    QWidget *newspapersListTab = new QWidget;

    /* optionsTab */
    QLabel *labelLanguage = new QLabel(tr("Select your language :"));
    labelLanguageNote = new QLabel;

    QComboBox *comboBoxLanguage = new QComboBox;
    comboBoxLanguage->addItem(tr("English"));
    comboBoxLanguage->addItem(tr("French"));
    comboBoxLanguage->setFixedWidth(100);
    
    if(settings.value("language").toString() == "fr_FR")
        comboBoxLanguage->setCurrentIndex(1);
    else
        comboBoxLanguage->setCurrentIndex(0);

    this->languageChanged(comboBoxLanguage->currentIndex());

    QFormLayout *languageFormLayout = new QFormLayout;
    languageFormLayout->setWidget(0, QFormLayout::LabelRole, labelLanguage);
    languageFormLayout->setWidget(0, QFormLayout::FieldRole, comboBoxLanguage);
    languageFormLayout->setWidget(1, QFormLayout::SpanningRole, labelLanguageNote);

    QGroupBox *languageBox = new QGroupBox("Language");
    languageBox->setLayout(languageFormLayout);

    QLabel *labelFolderNote = new QLabel(tr("<b>Note:</b> It's recommended to choose an empty folder."));

    lineEditCurrentFolder = new QLineEdit;
    lineEditCurrentFolder->setReadOnly(true);
    lineEditCurrentFolder->setText(newspapersFolder.path());
    lineEditCurrentFolder->setFixedSize(310, 20);
    QPushButton *pushButtonBrowse = new QPushButton("...");
    pushButtonBrowse->setFixedSize(20, 20);

    QFormLayout *folderFormLayout = new QFormLayout;
    folderFormLayout->setWidget(0, QFormLayout::LabelRole, lineEditCurrentFolder);
    folderFormLayout->setWidget(0, QFormLayout::FieldRole, pushButtonBrowse);
    folderFormLayout->setWidget(1, QFormLayout::SpanningRole, labelFolderNote);

    QGroupBox *folderBox = new QGroupBox(tr("Newspapers folder"));
    folderBox->setLayout(folderFormLayout);

    QVBoxLayout *optionsTabLayout = new QVBoxLayout;
    optionsTabLayout->addWidget(languageBox);
    optionsTabLayout->addWidget(folderBox, 6, Qt::AlignTop);

    optionsTab->setLayout(optionsTabLayout);

    /* newspapersListTab */
    QGridLayout *showHideGridLayout = new QGridLayout;

    QGroupBox *newspapersListBox = new QGroupBox(tr("Select the journals you want to use :"));
    newspapersListBox->setLayout(showHideGridLayout);

    QVBoxLayout *tab_NewsPapersSelectLayout = new QVBoxLayout;
    tab_NewsPapersSelectLayout->addWidget(newspapersListBox);

    newspapersListTab->setLayout(tab_NewsPapersSelectLayout);

    preferencesTabs->addTab(optionsTab, "Options");
    preferencesTabs->addTab(newspapersListTab, tr("NewsPapers List"));

    QPushButton *OK = new QPushButton("OK");
    OK->setFixedSize(60, 25);

    QVBoxLayout *DialogLayout = new QVBoxLayout(preferencesDialog);
    DialogLayout->addWidget(preferencesTabs);
    DialogLayout->addWidget(OK, 6, Qt::AlignRight);

    connect(pushButtonBrowse, SIGNAL(clicked()), this, SLOT(selectDir()));
    connect(OK, SIGNAL(clicked()), this, SLOT(XML_updater()));
    connect(OK, SIGNAL(clicked()), preferencesDialog, SLOT(close()));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(int)), this, SLOT(languageChanged(int)));

    loadNewspapersPreferencesList(showHideGridLayout);
    preferencesDialog->setFixedSize(0, 0);
    preferencesDialog->exec();
}

void mainwindow::loadNewspapersPreferencesList(QGridLayout *showHideGridLayout)
{
    for(int i = 0; i < newspaperCheckBoxs.size(); i++)
        newspaperCheckBoxs[i]->deleteLater();

    newspaperCheckBoxs.clear();

    QDomDocument DOM;

    file.setFileName(QCoreApplication::applicationDirPath() + "/newspapers.xml");
    file.open(QIODevice::ReadOnly);
    DOM.setContent(file.readAll());
    file.close();

    QDomNode Node;
    Node = DOM.documentElement().firstChild();

    int i = 0;//id
    int r = 0;//row
    int c = 0;//column

    while(Node.toElement().tagName() == "newspaper")
    {
        QDomElement Element = Node.toElement();

        QCheckBox *newspaperCheckBox = new QCheckBox(Element.attribute("name"));
        newspaperCheckBoxs.append(newspaperCheckBox);
        if(Element.attribute("hidden") == "false")
            newspaperCheckBox->setChecked(true);
        showHideGridLayout->addWidget(newspaperCheckBox, r, c);

        r++;
        i++;
        Node = Node.nextSibling();

        if(r == 15)
        {
            r = 0;
            c++;
        }
    }
    if(DOM.toString().isEmpty())
    {
        QLabel *emptyList = new QLabel(tr("No newspapers found !"));
        showHideGridLayout->addWidget(emptyList, r, c);
    }
}

void mainwindow::selectDir()
{
    QString path = QFileDialog::getExistingDirectory();

    if(!path.isEmpty())
    {
        newspapersFolder.setPath(path);
        lineEditCurrentFolder->setText(newspapersFolder.path());

        settings.setValue("newspapersFolder", path);
    }
}

void mainwindow::languageChanged(int index)
{
    switch(index)
    {
    case 1:
        settings.setValue("language", "fr_FR");
        labelLanguageNote->setText("<b>Note:</b> Redémarrer l'application pour prendre effet.");
        break;
    default:
        settings.setValue("language", "en_US");
        labelLanguageNote->setText("<b>Note:</b> Restart the application to take effect.");
        break;
    }
}

void mainwindow::XML_updater()
{
    QDomDocument DOM;

    file.setFileName(QCoreApplication::applicationDirPath() + "/newspapers.xml");
    file.open(QIODevice::ReadOnly);
    DOM.setContent(file.readAll());
    file.close();

    QDomNode Node;
    Node = DOM.documentElement().firstChild();

    int i = 0;
    while(Node.toElement().tagName() == "newspaper")
    {
        QDomElement Element = Node.toElement();

        if(newspaperCheckBoxs.at(i)->isChecked())
            Element.setAttribute("hidden","false");
        else
            Element.setAttribute("hidden","true");

        Node = Node.nextSibling();
        i++;
    }
    if(file.open(QFile::WriteOnly))
    {
        QTextStream out(&file);
        out << DOM.toString();
        file.close();
    }
    this->loadNewspapersList();
    pushButtonDownload->setEnabled(false);
    pushButtonOpen->setEnabled(false);
    lineEditLink->setText("http://qnewspaper.googlecode.com");
    currentNewspaperID = -1;
}
