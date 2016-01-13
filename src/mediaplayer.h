#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QtMultimedia/QMediaPlayer>

class MediaPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit MediaPlayer(QObject *parent = 0);

    Q_INVOKABLE void playSession(const QString &uri, const QString &lid);
    Q_INVOKABLE QString getSession() const;

private:
    QString _lid;

public slots:
};

#endif // MEDIAPLAYER_H
