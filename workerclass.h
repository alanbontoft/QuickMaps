#ifndef WORKERCLASS_H
#define WORKERCLASS_H

#include <QThread>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QMutex>
#include <QElapsedTimer>

class WorkerClass : public QThread
{
    Q_OBJECT
public:
    explicit WorkerClass(QObject *parent = nullptr);
    void startThread();

signals:
    void newDataReady(const QString &data);

private slots:
    void readData();

protected:
    void run();

private:
    QSerialPort *_port = nullptr;

    void closePort();
    bool openPort(QString port);

    QStringList _list;
    QString _store = "";

    QMutex _mutex;
    QElapsedTimer _timer;
    bool _running = false;

    QByteArray _buffer;
};

#endif // WORKERCLASS_H
