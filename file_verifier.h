#ifndef FILE_VERIFIER_H
#define FILE_VERIFIER_H

#include <QObject>
#include <QDebug>
#include <QPoint>
#include <QCryptographicHash>
#include <QFile>
class file_verifier : public QObject
{
    Q_OBJECT
public:
    explicit file_verifier(QObject *parent = 0);

    void setFileName(const QString&,int checkSumType);

    QByteArray getCheckSum() const;
signals:

public slots:

private:
    QString fileName;
    QByteArray checkSum;
};

#endif // FILE_VERIFIER_H
