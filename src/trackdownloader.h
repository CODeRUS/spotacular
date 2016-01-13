#ifndef TRACKDOWNLOADER_H
#define TRACKDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class TrackDownloader : public QObject
{
    Q_OBJECT
public:
    explicit TrackDownloader(const QString &userAgent, QObject *parent = 0);
    virtual ~TrackDownloader();

    void setData(const QString &from, const QString &to);

private:
    QString _userAgent;

    QString _source;
    QString _destination;

    QFile *_file;

    QNetworkAccessManager *_nam;

    qint64 _read;
    qint64 _total;

public slots:
    void download();

private slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onError(QNetworkReply::NetworkError error);
    void onFinished();
    void onReadyRead();
    void onSslErrors(const QList<QSslError> &errors);

signals:
    void progress(float value);
    void completed(const QString &dest, TrackDownloader *object);
    void failed(TrackDownloader *object);
};

#endif // TRACKDOWNLOADER_H
