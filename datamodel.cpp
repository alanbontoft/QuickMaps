#include "datamodel.h"

DataModel::DataModel(QObject *parent)
    : QObject{parent}
{
    _worker = new WorkerClass();

    connect(_worker, &QThread::finished, this, &DataModel::onThreadFinished);

    connect(_worker, &WorkerClass::newDataReady, this, &DataModel::onNewData);

    runThread();
}

QString DataModel::time() const
{
    return _time;
}

void DataModel::setTime(const QString &newTime)
{
    if (_time == newTime)
        return;
    _time = newTime;
    emit timeChanged();
}

void DataModel::runThread()
{
    _worker->startThread();
}

void DataModel::onNewData(const QString &data)
{
    qDebug() << "New data: " << data;
    parseData(data);
}

void DataModel::parseData(const QString &s)
{
    auto list = s.split(',');
    double lat, lon;

    if (list.length() >= 6)
    {
        auto timeData = list[1];
        qDebug() << "Time: " << timeData;
        setTime(QString("%1:%2:%3").arg(timeData.mid(0, 2)).arg(timeData.mid(2, 2)).arg(timeData.mid(4, 2)));
        QString latitude = list[2];
        qDebug() << "Latitude: " << latitude;
        auto latitudeDir = list[3];
        qDebug() << "Latitude Dir: " << latitudeDir;
        auto longitude = list[4];
        qDebug() << "Longitude: " << longitude;
        auto longitudeDir = list[5];
        qDebug() << "Longitude Dir: " << longitudeDir;

        lat = latitude.mid(0, 2).toDouble();
        lat += (latitude.mid(2).toDouble() / 60.0);
        if (latitudeDir != "N") lat *= -1.0;

        lon = longitude.mid(0, 3).toDouble();
        lon += (longitude.mid(3).toDouble() / 60.0);
        if (longitudeDir != "E") lon *= -1.0;

        emit coordsReceived(lat, lon);

    }
}
