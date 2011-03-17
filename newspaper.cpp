#include "newspaper.h"

qRadioButton::qRadioButton(QString name, QString website, QString filelocation, QString today, QString extension, QString type, QString regexp, int i)
{
    m_name = name;
    m_website = website;
    m_filelocation = filelocation;
    m_today = today;
    m_extension = extension;
    m_type = type.toInt();
    m_regexp.setPattern(regexp);
    ID = i;

    this->setText(m_name);
}

QString qRadioButton::getUrl()
{
    switch(m_type)
    {
    case 0:
        return "http://" + m_website + dateConvert(m_filelocation) + "." + m_extension;
        break;
    case 1:
        return "http://" + m_website + m_filelocation;
        break;
    case 2:
        return "http://" + m_website + m_filelocation + "." + m_extension;
        break;
    case 3:
        return "http://" + m_website + dateConvert(m_filelocation) + "." + m_extension;
        break;
    }
    return 0;
}

QString qRadioButton::getTodayUrl()
{
    return "http://" + m_website + m_today + "." + m_extension;
}

QString qRadioButton::dateConvert(QString str)
{
    QDate date = emit getDate();
    QString upper;

    //We have to use QLocale::French

    upper = date.toString("dddd");
    upper[0] = upper[0].toUpper();

    str.replace("dddd", upper);
    str.replace("MMMM", date.toString("MMMM"));
    str.replace("%20d%20", date.toString("%20d%20"));
    str.replace("dd", date.toString("dd"));
    str.replace("MM", date.toString("MM"));
    str.replace("yyyy", date.toString("yyyy"));
    str.replace("yy", date.toString("yy"));

    return str;
}

int qRadioButton::getType()
{
    return m_type;
}

int qRadioButton::getID()
{
    return ID;
}

QString qRadioButton::getFilename()
{
    return m_name + "." + m_extension;
}

QString qRadioButton::getName()
{
    return m_name;
}

QString qRadioButton::getExtension()
{
    return m_extension;
}

QRegExp qRadioButton::getRx()
{
    return m_regexp;
}

qRadioButton::~qRadioButton()
{

}
