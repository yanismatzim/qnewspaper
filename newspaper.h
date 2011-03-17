#ifndef NEWSPAPER_H
#define NEWSPAPER_H

#include <QRadioButton>
#include <QDate>

class qRadioButton : public QRadioButton
{
    Q_OBJECT

public:
    qRadioButton(QString name, QString website, QString filelocation, QString today, QString extension, QString type, QString regexp, int i);
    QString dateConvert(QString str);
    QString getUrl();
    QString getTodayUrl();
    int getID();
    int getType();
    QString getName();
    QString getExtension();
    QString getFilename();
    QRegExp getRx();
    ~qRadioButton();

signals:
    QDate getDate();

private:
    QString m_name;
    QString m_website;
    QString m_filelocation;
    int m_type;
    QString m_extension;
    QRegExp m_regexp;
    QString m_today;
    int ID;
};

#endif // NEWSPAPER_H
