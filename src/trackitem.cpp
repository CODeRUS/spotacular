#include "trackitem.h"

TrackItem::TrackItem(const QVariantMap &data, QObject *parent):
    QObject(parent),
    _data(data)
{
    SpotifyIO::GetInstance()->getMetadata(
            _data["type"].toString(),
            QStringList() << _data["id"].toString(),
            this,
            COMMANDCALLBACK(onMetadataReceived));
}

QVariant TrackItem::getData(const QString &key)
{
    return _data[key];
}

void TrackItem::onMetadataReceived(const QVariant &data)
{
    QStringList metadataList = data.toStringList();
    QString metadataHeader = metadataList.first();
    QByteArray byteData = QByteArray::fromBase64(metadataHeader.toLatin1());
    spotify::mercury::proto::MercuryReply mercuryReply;
    mercuryReply.ParseFromArray(byteData.constData(), byteData.size());
    ::std::string replyStatus = mercuryReply.status_message();
    QByteArray contentType(replyStatus.data(), replyStatus.length());
    if (contentType == "vnd.spotify/metadata-track") {
        QByteArray itemData(QByteArray::fromBase64(metadataList[1].toLatin1()));
        spotify::metadata::proto::Track track;
        track.ParseFromArray(itemData.constData(), itemData.size());

        ::std::string trackGid = track.gid();
        QByteArray gid(trackGid.data(), trackGid.length());

        qDebug() << "files available for" << SpotifyUtils::gid2id(QString::fromLatin1(gid));
        for (int i = 0; i < track.file_size(); i++) {
            ::std::string s_fileId = track.file(i).file_id();
            QByteArray fileId(s_fileId.data(), s_fileId.length());
            qDebug() << SpotifyUtils::gid2id(QString::fromLatin1(fileId));
            switch (track.file(i).format()) {
                case spotify::metadata::proto::AudioFile_Format_MP3_96:
                    qDebug() << "MP3/96";
                    break;
                case spotify::metadata::proto::AudioFile_Format_MP3_160:
                    qDebug() << "MP3/160";
                    break;
                case spotify::metadata::proto::AudioFile_Format_MP3_320:
                    qDebug() << "MP3/320";
                    break;
                case spotify::metadata::proto::AudioFile_Format_MP3_256:
                    qDebug() << "MP3/256";
                    break;
                case spotify::metadata::proto::AudioFile_Format_OGG_VORBIS_320:
                    qDebug() << "OGG/320";
                    break;
                case spotify::metadata::proto::AudioFile_Format_OGG_VORBIS_160:
                    qDebug() << "OGG/160";
                    break;
                case spotify::metadata::proto::AudioFile_Format_OGG_VORBIS_96:
                    qDebug() << "OGG/96";
                    break;
            }
        }

        /*SpotifyIO::GetInstance()->sendCommand(
                    "sp/track_uri",
                    QVariantList()
                        << "mp3160"
                        << SpotifyUtils::gid2id(QString::fromLatin1(gid)),
                    this,
                    COMMANDCALLBACK(onTrackUriReceived));*/
    }
    else {
        qDebug() << "ERROR" << contentType;
    }
}

void TrackItem::onTrackUriReceived(const QVariant &data)
{
    qDebug() << data;
}

