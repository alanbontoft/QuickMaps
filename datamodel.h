#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QDebug>
#include <QThread>

#include "workerclass.h"

class DataModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged)

Q_SIGNALS:
    void coordsReceived(const double &latitude, const double &longitude);

public:
    explicit DataModel(QObject *parent = nullptr);

    QString time() const;
    void setTime(const QString &newTime);

signals:
    void timeChanged();

private slots:
    void onThreadFinished() { qDebug() << "Thread finished"; }
    void onNewData(const QString &data);
private:
    QString _time;

    WorkerClass *_worker;

    void parseData(const QString &s);
    void runThread();
};

#endif // DATAMODEL_H
