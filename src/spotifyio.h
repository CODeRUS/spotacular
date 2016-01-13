#ifndef SPOTIFYIO_H
#define SPOTIFYIO_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QtWebSockets/QWebSocket>

#include <QTimer>

#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include "mercury.pb.h"
#include "metadata.pb.h"
#include "playlist4changes.pb.h"
#include "playlist4content.pb.h"

#include "mediaplayer.h"
#include "trackitem.h"

#define COMMANDCALLBACK(a) #a

class SpotifyIO : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
public:
    enum State {
        DisconnectedState = 0,
        ConnectingState,
        AuthorizingState,
        LoggedInState
    };

    explicit SpotifyIO(QObject *parent = 0);
    virtual ~SpotifyIO();

    static SpotifyIO *GetInstance(QObject *parent = 0);

    Q_INVOKABLE void login(const QString &username, const QString &password);
    Q_INVOKABLE void closeSocket();

    Q_INVOKABLE QVariant getUserData(const QString &key);

    Q_INVOKABLE void search(const QString &keyword, int searchType = 15, int maxResults, int offset, QObject *receiver, const char *callback);
    Q_INVOKABLE void playSong(const QString &uritype, const QStringList &ids);
    Q_INVOKABLE void download(const QString &uritype, const QStringList &ids);

    State state() const;

private:
    friend class TrackItem;
    void sendCommand(const QString &name, const QVariantList &args);
    void sendCommand(const QString &name, const QVariantList &args, QObject *receiver, const char* callback);

    void getMetadata(const QString &uritype, const QStringList &ids, QObject *receiver, const char* callback);

    void getPlaylists(const QString &user, QObject *receiver, const char *callback);
    void getPlaylist(const QString &user, const QString &uri, QObject *receiver, const char *callback);

    void _makeLandingPageRequest();
    void _makeLoginRequest(const QByteArray &username, const QByteArray &password, const QVariantMap &landingCreds);
    QString _constructPong(const QString &ping);

    void sendTrackEnd();

    QByteArray makeForm(const QVariantMap &formData);

    QNetworkAccessManager *nam;
    QWebSocket *ws;
    QTimer *heartbeat;

    MediaPlayer *mediaPlayer;

    QVariantMap currentTrack;
    QVariantMap pendingTrack;

protected:
    QString username;
    QString password;
    QVariantMap settings;

    int seq;
    QReadWriteLock lock;

    State _state;

    QHash<int, const char*> callbacks;
    QHash<int, QObject*> receivers;

    QVariantMap userInfo;

private slots:
    void landingFilished();
    void loginReply();
    void resolverReply();
    void onPongReply();

    void onHeartbeat();

    void onConnected();
    void onDisconnected();
    void onStateChanged(QAbstractSocket::SocketState state);
    void onTextMessageReceived(const QString &message);
    void onBinaryMessageReceived(const QByteArray &message);
    void onError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);

    void onUserInfo(const QVariant &data);
    void onEchoReply(const QVariant &data);
    void onMyPlaylists(const QVariant &data);
    void onPlaylistContents(const QVariant &data);

    void searchReply(const QVariant &data);
    void parseMetadataToPlay(const QVariant &data);
    void parseMetadataToDownload(const QVariant &data);
    void parseMetadata(const QVariant &data, QObject *receiver, const char *callback);
    void parseMetadataItem(const QByteArray &contentType, const QByteArray &data, QObject *receiver, const char* callback);
    void trackUriPlay(const QVariant &data);
    void trackUriDownload(const QVariant &data);

signals:
    void authFailed(const QString &reason = QString());
    void searchCompleted(const QString &data);

    void stateChanged();

};

#endif // SPOTIFYIO_H
