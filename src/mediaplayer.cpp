#include "mediaplayer.h"
#include <QDebug>

MediaPlayer::MediaPlayer(QObject *parent) : QMediaPlayer(parent)
{
}

void MediaPlayer::playSession(const QString &uri, const QString &lid)
{
    if (this->state() == QMediaPlayer::PlayingState) {
        this->stop();
    }

    _lid = lid;

    setMedia(QUrl(uri));
    play();
}

QString MediaPlayer::getSession() const
{
    qDebug() << duration();
    return _lid;
}

