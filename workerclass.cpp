#include "workerclass.h"

WorkerClass::WorkerClass(QObject *parent)
    : QThread{parent}
{
    if (!openPort("/dev/ttyUSB0")) { qDebug() << "Failed to open port"; }

    connect(_port, &QSerialPort::readyRead, this, &WorkerClass::readData);

    _buffer.clear();
}

void WorkerClass::startThread()
{
    _running = true;
    this->start();
}

void WorkerClass::readData()
{
    _mutex.lock();

    _buffer.append(_port->readAll());

    _mutex.unlock();
}


void WorkerClass::run()
{
    QString newData;
    bool moreData;
    int index, count;

    _store = "";
    _list.clear();

    while (_running)
    {

        if (_buffer.length() > 0)
        {
            _mutex.lock();

            moreData = true;
            count = 0;

            QString data(_buffer);

            while (moreData)
            {
                index = data.indexOf('\n');

                if (index != -1)
                {
                    if (index == 0)
                    {
                        newData = _store.removeLast();
                        _store = "";
                        count++;
                    }
                    else
                    {
                        newData = data.first(index - 1);
                        count += index + 1;

                        if (_store.length() > 0)
                        {
                            newData = _store + newData;
                            _store = "";
                        }
                    }

                    data.remove(0, index + 1);

                    if (newData.length() > 6)
                    {
                        if (newData.first(6) == "$GPGGA")
                        {
                            _list.append(newData);
                            emit newDataReady(newData);
                        }
                    }
                }
                else
                {
                    moreData = false;
                    _store = _store + data;
                    count += data.length();
                }
            }

            _buffer.remove(0, count);

            _mutex.unlock();
        }

    }

}

void WorkerClass::closePort()
{
    if (_port != nullptr)
    {
        _port->close();
        delete _port;
        _port = nullptr;
    }
}

bool WorkerClass::openPort(QString port)
{
    closePort();

    _port = new QSerialPort(this);
    _port->setPortName(port);
    _port->setBaudRate(9600);
    _port->setDataBits(QSerialPort::Data8);
    _port->setParity(QSerialPort::NoParity);
    return _port->open(QIODevice::ReadWrite);
}
