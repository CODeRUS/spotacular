#include "trackdownloader.h"

TrackDownloader::TrackDownloader(const QString &userAgent, QObject *parent) :
    QObject(parent),
    _userAgent(userAgent)
{
    _file = NULL;
    _nam = NULL;
}

TrackDownloader::~TrackDownloader()
{

}

void TrackDownloader::setData(const QString &from, const QString &to)
{
    _source = from;
    _destination = to;
    _read = 0;
    _total = 0;
}

void TrackDownloader::download()
{
    qDebug() << _source << _destination << _userAgent;
    _file = new QFile(_destination, this);
    if (_file->open(QFile::WriteOnly)) {
        _file->resize(0);
        _file->seek(0);
        _nam = new QNetworkAccessManager(this);
        QUrl url(_source);
        QNetworkRequest req(url);
        req.setRawHeader("User-Agent", _userAgent.toLatin1());
        req.setRawHeader("Connection", "closed");
        QNetworkReply *reply = _nam->get(req);
        QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
        QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
        QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        QObject::connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));
    }
    else {
        qWarning() << "Can't open file:" << _destination;
    }
}

void TrackDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << bytesReceived << bytesTotal;
    if (_total == 0) {
        _total = bytesTotal;
    }
    Q_EMIT progress((float)bytesReceived / bytesTotal);
}

void TrackDownloader::onError(QNetworkReply::NetworkError error)
{
    qDebug() << error;

    _file->flush();
    _file->close();
    _file->remove();

    Q_EMIT failed(this);
}

void TrackDownloader::onFinished()
{
    qDebug() << "";

    _file->flush();
    _file->close();

    Q_EMIT completed(_destination, this);
}

void TrackDownloader::onReadyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        qint64 bytes = reply->bytesAvailable();
        _read += bytes;
        qDebug() << bytes << _read << _total;
        _file->write(reply->readAll());
        if (_total > 0) {
            //Q_EMIT progress(_jid, (float)_read / _total, _msgid);
        }
    }
}

void TrackDownloader::onSslErrors(const QList<QSslError> &errors)
{
    qDebug() << errors;

    _file->flush();
    _file->close();
    _file->remove();

    Q_EMIT failed(this);
}
