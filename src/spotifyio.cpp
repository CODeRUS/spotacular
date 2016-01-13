#include "spotifyio.h"

#include <QJsonDocument>
#include <QUrlQuery>
#include <QDebug>

#include <QScriptEngine>
#include <QScriptValue>

#include "spotifyutils.h"
#include "trackdownloader.h"

const QString authServer("play.spotify.com");
const QString authUrl("/xhr/json/auth.php");
const QString landingUrl("/");
const QString userAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36");
const QString sourceUrl("https://d3rt1990lpmkn.cloudfront.net");
const QString origin("https://play.spotify.com");

static QScriptValue reply(QScriptContext *context, QScriptEngine *)
{
    return context->argument(0);
}

SpotifyIO::SpotifyIO(QObject *parent) :
    QObject(parent)
{
    nam = new QNetworkAccessManager(this);
    ws = new QWebSocket(origin, QWebSocketProtocol::VersionLatest, this);
    mediaPlayer = new MediaPlayer(this);

    QObject::connect(ws, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(ws, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(ws, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    QObject::connect(ws, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
    QObject::connect(ws, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));
    QObject::connect(ws, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    QObject::connect(ws, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));

    heartbeat = new QTimer(this);
    heartbeat->setInterval(3 * 60 * 1000);
    QObject::connect(heartbeat, SIGNAL(timeout()), this, SLOT(onHeartbeat()));

    _state = DisconnectedState;
    Q_EMIT stateChanged();
}

SpotifyIO::~SpotifyIO()
{
    if (ws->state() == QAbstractSocket::ConnectedState) {
        ws->close();
    }
}

SpotifyIO *SpotifyIO::GetInstance(QObject *parent)
{
    static SpotifyIO* lsSingleton = NULL;
    if (!lsSingleton) {
        lsSingleton = new SpotifyIO(parent);
    }
    return lsSingleton;
}

void SpotifyIO::login(const QString &username, const QString &password)
{
    this->username = username;
    this->password = password;

    _state = ConnectingState;
    Q_EMIT stateChanged();

    _makeLandingPageRequest();
}

void SpotifyIO::closeSocket()
{
    ws->close();
}

QVariant SpotifyIO::getUserData(const QString &key)
{
    return userInfo[key];
}

void SpotifyIO::search(const QString &keyword, int searchType, int maxResults, int offset, QObject *receiver, const char *callback)
{
    sendCommand("sp/search",
                QVariantList()
                << keyword
                << searchType
                << maxResults
                << offset,
                receiver,
                callback);
}

void SpotifyIO::playSong(const QString &uritype, const QStringList &ids)
{
    getMetadata(uritype, ids, this, COMMANDCALLBACK(parseMetadataToPlay));
}

void SpotifyIO::download(const QString &uritype, const QStringList &ids)
{
    getMetadata(uritype, ids, this, COMMANDCALLBACK(parseMetadataToDownload));
}

SpotifyIO::State SpotifyIO::state() const
{
    return _state;
}

void SpotifyIO::sendCommand(const QString &name, const QVariantList &args)
{
    QWriteLocker locker(&lock);

    QVariantMap msg;
    msg["name"] = name;
    msg["id"] = QString::number(seq++);
    msg["args"] = args;

    QByteArray data = QJsonDocument::fromVariant(msg).toJson(QJsonDocument::Compact);

    //qDebug() << data;

    ws->sendTextMessage(QString(data));
}

void SpotifyIO::sendCommand(const QString &name, const QVariantList &args, QObject *receiver, const char *callback)
{
    //qDebug() << "storing callback" << callback << "for" << seq;
    QWriteLocker locker(&lock);

    receivers[seq] = receiver;
    callbacks[seq] = callback;

    QVariantMap msg;
    msg["name"] = name;
    msg["id"] = QString::number(seq++);
    msg["args"] = args;

    QByteArray data = QJsonDocument::fromVariant(msg).toJson(QJsonDocument::Compact);

    //qDebug() << data;

    ws->sendTextMessage(QString(data));
}

void SpotifyIO::getMetadata(const QString &uritype, const QStringList &ids, QObject *receiver, const char *callback)
{
    spotify::mercury::proto::MercuryMultiGetRequest mercuryRequests;

    foreach (const QString &id, ids) {
        if (uritype == "local") {
            qWarning() << "Track with id" << id << "is a local track, we can't request metadata, skipping";
            continue;
        }
        spotify::mercury::proto::MercuryRequest mercuryRequest;
        mercuryRequest.set_method("GET");
        QString reqUri = QString("hm://metadata/%1/%2").arg(uritype).arg(id);
        mercuryRequest.set_uri(reqUri.toLatin1().constData(), reqUri.size());
        mercuryRequests.add_request()->CopyFrom(mercuryRequest);
    }

    QString uri = QString("hm://metadata/%1s").arg(uritype);

    QVariantList args;
    args << 0;

    if (mercuryRequests.request_size() == 1) {
        ::std::string serialized = mercuryRequests.request(0).SerializeAsString();
        QByteArray serializedReq(serialized.data(), serialized.length());
        args << serializedReq.toBase64();
    }
    else {
        spotify::mercury::proto::MercuryRequest header;
        header.set_method("GET");
        header.set_uri(uri.toLatin1().constData());
        header.set_content_type("vnd.spotify/mercury-mget-request");
        ::std::string headerStr = header.SerializeAsString();
        QByteArray serializedHeader(headerStr.data(), headerStr.length());
        args << serializedHeader.toBase64();
        ::std::string requestsStr = mercuryRequests.SerializeAsString();
        QByteArray serializedRequests(requestsStr.data(), requestsStr.length());
        args << serializedRequests.toBase64();
    }

    sendCommand("sp/hm_b64", args, receiver, callback);
}

void SpotifyIO::getPlaylists(const QString &user, QObject *receiver, const char* callback)
{
    spotify::mercury::proto::MercuryRequest mercuryRequest;
    mercuryRequest.set_method("GET");
    QString reqUri = QString("hm://playlist/user/%1/publishedrootlist?from=0&length=100").arg(user);
    mercuryRequest.set_uri(reqUri.toLatin1().constData(), reqUri.size());

    QByteArray serializedRequest(mercuryRequest.ByteSize(), '\0');
    mercuryRequest.SerializeToArray(serializedRequest.data(), mercuryRequest.ByteSize());

    QVariantList args;
    args << 0;
    args << serializedRequest.toBase64();

    sendCommand("sp/hm_b64", args, receiver, callback);
}

void SpotifyIO::getPlaylist(const QString &user, const QString &uri, QObject *receiver, const char *callback)
{
    spotify::mercury::proto::MercuryRequest mercuryRequest;
    mercuryRequest.set_method("GET");
    QString reqUri = QString("hm://playlist/user/%1/playlist/%2?from=0&length=100").arg(user).arg(uri);
    mercuryRequest.set_uri(reqUri.toLatin1().constData(), reqUri.size());

    QByteArray serializedRequest(mercuryRequest.ByteSize(), '\0');
    mercuryRequest.SerializeToArray(serializedRequest.data(), mercuryRequest.ByteSize());

    QVariantList args;
    args << 0;
    args << serializedRequest.toBase64();

    sendCommand("sp/hm_b64", args, receiver, callback);
}

void SpotifyIO::_makeLandingPageRequest()
{
    qDebug() << "tokens request";
    QUrl url;
    url.setScheme("https");
    url.setHost(authServer);
    url.setPath(landingUrl);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    QObject::connect(nam->get(req), SIGNAL(finished()), this, SLOT(landingFilished()));
}

void SpotifyIO::_makeLoginRequest(const QByteArray &username, const QByteArray &password, const QVariantMap &landingCreds)
{
    qDebug() << "login request";
    QVariantMap formData;
    formData["username"] = username;
    formData["password"] = password;
    formData["type"] = "sp";
    formData["secret"] = landingCreds["csrftoken"];
    formData["trackingId"] = landingCreds["trackingId"];
    formData["landingURL"] = landingCreds["landingURL"];
    formData["referrer"] = landingCreds["referrer"];
    formData["cf"] = QVariant();

    QByteArray data = makeForm(formData);

    QUrl url;
    url.setScheme("https");
    url.setHost(authServer);
    url.setPath(authUrl);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    QObject::connect(nam->post(req, data), SIGNAL(finished()), this, SLOT(loginReply()));
}

QString SpotifyIO::_constructPong(const QString &ping)
{
    QList<int> pingList;
    foreach (const QString &pingItem, ping.split(" ")) {
        pingList.append(pingItem.toInt());
    }
    QStringList pong;
    pong.append(QString::number(pingList[8]^146));
    QStringList list14;
    list14 << "54" << "55" << "56" << "56" << "57" << "58" << "59" << "60" << "61" << "62" << "62" << "63" << "64" << "65" << "66" << "67" << "68" << "68" << "69" << "70" << "71" << "72" << "73" << "73" << "74" << "75" << "76" << "77" << "78" << "79" << "79" << "80" << "81" << "82" << "83" << "84" << "85" << "85" << "86" << "87" << "88" << "89" << "90" << "90" << "91" << "92" << "93" << "94" << "95" << "96" << "96" << "97" << "98" << "99" << "100" << "101" << "102" << "102" << "103" << "104" << "105" << "106" << "107" << "107" << "0" << "0" << "1" << "2" << "3" << "4" << "5" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "11" << "12" << "13" << "14" << "15" << "16" << "17" << "17" << "18" << "19" << "20" << "21" << "22" << "22" << "23" << "24" << "25" << "26" << "27" << "28" << "28" << "29" << "30" << "31" << "32" << "33" << "34" << "34" << "35" << "36" << "37" << "38" << "39" << "39" << "40" << "41" << "42" << "43" << "44" << "45" << "45" << "46" << "47" << "48" << "49" << "50" << "51" << "51" << "52" << "53" << "163" << "164" << "164" << "165" << "166" << "167" << "168" << "169" << "170" << "170" << "171" << "172" << "173" << "174" << "175" << "175" << "176" << "177" << "178" << "179" << "180" << "181" << "181" << "182" << "183" << "184" << "185" << "186" << "187" << "187" << "188" << "189" << "190" << "191" << "192" << "192" << "193" << "194" << "195" << "196" << "197" << "198" << "198" << "199" << "200" << "201" << "202" << "203" << "204" << "204" << "205" << "206" << "207" << "208" << "209" << "209" << "210" << "211" << "212" << "213" << "214" << "215" << "215" << "216" << "108" << "109" << "110" << "111" << "112" << "113" << "113" << "114" << "115" << "116" << "117" << "118" << "119" << "119" << "120" << "121" << "122" << "123" << "124" << "124" << "125" << "126" << "127" << "128" << "129" << "130" << "130" << "131" << "132" << "133" << "134" << "135" << "136" << "136" << "137" << "138" << "139" << "140" << "141" << "141" << "142" << "143" << "144" << "145" << "146" << "147" << "147" << "148" << "149" << "150" << "151" << "152" << "153" << "153" << "154" << "155" << "156" << "157" << "158" << "158" << "159" << "160" << "161" << "162";
    pong.append(list14[pingList[14]]);
    pong.append(QString::number(pingList[17]^173));
    pong.append(QString::number(pingList[5]^4));
    pong.append(QString::number(pingList[10]^178));
    pong.append(QString::number(pingList[6]^200));
    pong.append(QString::number(pingList[4]^124));
    QStringList list3;
    list3 << "232" << "231" << "230" << "229" << "236" << "235" << "234" << "233" << "224" << "223" << "222" << "221" << "228" << "227" << "226" << "225" << "248" << "247" << "246" << "245" << "252" << "251" << "250" << "249" << "240" << "239" << "238" << "237" << "244" << "243" << "242" << "241" << "200" << "199" << "198" << "197" << "204" << "203" << "202" << "201" << "192" << "191" << "190" << "189" << "196" << "195" << "194" << "193" << "216" << "215" << "214" << "213" << "220" << "219" << "218" << "217" << "208" << "207" << "206" << "205" << "212" << "211" << "210" << "209" << "29" << "29" << "29" << "29" << "30" << "29" << "29" << "29" << "28" << "28" << "28" << "28" << "29" << "29" << "29" << "28" << "31" << "31" << "31" << "30" << "31" << "31" << "31" << "31" << "30" << "30" << "30" << "30" << "30" << "30" << "30" << "30" << "26" << "26" << "26" << "26" << "26" << "26" << "26" << "26" << "25" << "255" << "254" << "253" << "26" << "25" << "25" << "25" << "28" << "27" << "27" << "27" << "28" << "28" << "28" << "28" << "27" << "27" << "27" << "26" << "27" << "27" << "27" << "27" << "36" << "35" << "35" << "35" << "36" << "36" << "36" << "36" << "35" << "35" << "35" << "34" << "35" << "35" << "35" << "35" << "37" << "37" << "37" << "37" << "38" << "37" << "37" << "37" << "36" << "36" << "36" << "36" << "37" << "37" << "37" << "36" << "32" << "32" << "32" << "32" << "33" << "33" << "33" << "32" << "32" << "31" << "31" << "31" << "32" << "32" << "32" << "32" << "34" << "34" << "34" << "34" << "34" << "34" << "34" << "34" << "33" << "33" << "33" << "33" << "34" << "33" << "33" << "33" << "42" << "42" << "42" << "42" << "42" << "42" << "42" << "42" << "41" << "41" << "41" << "41" << "42" << "41" << "41" << "41" << "44" << "43" << "43" << "43" << "44" << "44" << "44" << "44" << "43" << "43" << "43" << "42" << "43" << "43" << "43" << "43" << "39" << "39" << "39" << "38" << "39" << "39" << "39" << "39" << "38" << "38" << "38" << "38" << "38" << "38" << "38" << "38" << "40" << "40" << "40" << "40" << "41" << "41" << "41" << "40" << "40" << "39" << "39" << "39" << "40" << "40" << "40" << "40";
    pong.append(list3[pingList[3]]);
    QStringList list7;
    list7 << "6" << "5" << "7" << "7" << "9" << "8" << "10" << "9" << "0" << "0" << "2" << "1" << "3" << "2" << "4" << "4" << "17" << "16" << "18" << "18" << "20" << "19" << "21" << "21" << "11" << "11" << "13" << "12" << "14" << "14" << "16" << "15" << "28" << "28" << "30" << "29" << "31" << "30" << "32" << "32" << "23" << "22" << "24" << "23" << "25" << "25" << "27" << "26" << "39" << "39" << "41" << "40" << "42" << "42" << "44" << "43" << "34" << "33" << "35" << "35" << "37" << "36" << "38" << "37" << "51" << "50" << "52" << "51" << "53" << "53" << "55" << "54" << "45" << "44" << "46" << "46" << "48" << "47" << "49" << "49" << "62" << "61" << "63" << "63" << "65" << "64" << "66" << "65" << "56" << "56" << "58" << "57" << "59" << "58" << "60" << "60" << "73" << "72" << "74" << "74" << "76" << "75" << "77" << "77" << "67" << "67" << "69" << "68" << "70" << "70" << "72" << "71" << "84" << "84" << "86" << "85" << "87" << "86" << "88" << "88" << "79" << "78" << "80" << "79" << "81" << "81" << "83" << "82" << "95" << "95" << "97" << "96" << "98" << "98" << "100" << "99" << "90" << "89" << "91" << "91" << "93" << "92" << "94" << "93" << "107" << "106" << "108" << "107" << "109" << "109" << "111" << "110" << "101" << "100" << "102" << "102" << "104" << "103" << "105" << "105" << "118" << "117" << "119" << "119" << "121" << "120" << "122" << "121" << "112" << "112" << "114" << "113" << "115" << "114" << "116" << "116" << "129" << "128" << "130" << "130" << "132" << "131" << "133" << "133" << "123" << "123" << "125" << "124" << "126" << "126" << "128" << "127" << "140" << "140" << "142" << "141" << "143" << "142" << "144" << "144" << "135" << "134" << "136" << "135" << "137" << "137" << "139" << "138" << "151" << "151" << "153" << "152" << "154" << "154" << "156" << "155" << "146" << "145" << "147" << "147" << "149" << "148" << "150" << "149" << "163" << "162" << "164" << "163" << "165" << "165" << "167" << "166" << "157" << "156" << "158" << "158" << "160" << "159" << "161" << "161" << "174" << "173" << "175" << "175" << "177" << "176" << "178" << "177" << "168" << "168" << "170" << "169" << "171" << "170" << "172" << "172";
    pong.append(list7[pingList[7]]);
    QStringList list12;
    list12 << "65" << "68" << "60" << "63" << "75" << "78" << "70" << "73" << "45" << "47" << "40" << "42" << "55" << "57" << "50" << "52" << "25" << "27" << "20" << "22" << "35" << "37" << "30" << "32" << "5" << "7" << "0" << "2" << "15" << "17" << "10" << "12" << "146" << "148" << "141" << "143" << "156" << "158" << "151" << "153" << "126" << "128" << "120" << "123" << "136" << "138" << "131" << "133" << "105" << "108" << "100" << "103" << "115" << "118" << "110" << "113" << "85" << "88" << "80" << "83" << "95" << "98" << "90" << "93" << "226" << "229" << "221" << "224" << "236" << "239" << "231" << "234" << "206" << "209" << "201" << "204" << "216" << "219" << "211" << "214" << "186" << "189" << "181" << "183" << "196" << "199" << "191" << "194" << "166" << "168" << "161" << "163" << "176" << "178" << "171" << "173" << "30" << "30" << "30" << "30" << "31" << "32" << "31" << "31" << "28" << "28" << "28" << "28" << "29" << "29" << "29" << "29" << "26" << "26" << "26" << "26" << "27" << "27" << "27" << "27" << "246" << "249" << "241" << "244" << "25" << "25" << "252" << "254" << "38" << "39" << "38" << "38" << "39" << "40" << "39" << "39" << "36" << "37" << "36" << "36" << "37" << "38" << "37" << "37" << "34" << "35" << "34" << "34" << "35" << "36" << "35" << "35" << "32" << "33" << "32" << "32" << "33" << "34" << "33" << "33" << "46" << "47" << "46" << "46" << "47" << "48" << "47" << "47" << "44" << "45" << "44" << "44" << "45" << "46" << "45" << "45" << "42" << "43" << "42" << "42" << "43" << "44" << "43" << "43" << "40" << "41" << "40" << "40" << "41" << "42" << "41" << "41" << "54" << "55" << "54" << "54" << "55" << "56" << "55" << "55" << "52" << "53" << "52" << "52" << "53" << "54" << "53" << "53" << "50" << "51" << "50" << "50" << "51" << "52" << "51" << "51" << "48" << "49" << "48" << "48" << "49" << "50" << "49" << "49" << "63" << "63" << "62" << "62" << "64" << "64" << "63" << "63" << "60" << "61" << "60" << "60" << "61" << "62" << "61" << "61" << "58" << "59" << "58" << "58" << "59" << "60" << "59" << "59" << "56" << "57" << "56" << "56" << "57" << "58" << "57" << "57";
    pong.append(list12[pingList[12]]);
    return pong.join(" ");
}

void SpotifyIO::sendTrackEnd()
{
    int ms_played = mediaPlayer->duration();
    int ms_played_union = ms_played;
    int n_seeks_forward = 0;
    int n_seeks_backward = 0;
    int ms_seeks_forward = 0;
    int ms_seeks_backward = 0;
    int ms_latency = 100;
    bool display_track = false;
    QString play_context = "unknown";
    QString source_start = "unknown";
    QString source_end = "unknown";
    QString reason_start = "unknown";
    QString reason_end = "unknown";
    QString referrer = "unknown";
    QString referrer_version = "0.1.0";
    QString referrer_vendor = "com.spotify";
    int max_continuous = ms_played;
    sendCommand("sp/track_end",
                QVariantList()
                << mediaPlayer->getSession()
                << ms_played
                << ms_played_union
                << n_seeks_forward
                << n_seeks_backward
                << ms_seeks_forward
                << ms_seeks_backward
                << ms_latency
                << display_track
                << play_context
                << source_start
                << source_end
                << reason_start
                << reason_end
                << referrer
                << referrer_version
                << referrer_vendor
                << max_continuous);
}

void SpotifyIO::landingFilished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        QByteArray page(reply->readAll());
        int offset0 = page.indexOf("Spotify.Web.Login");
        int offset1 = page.indexOf("Spotify.Web.App.initialize");
        page = page.mid(offset0, offset1);
        int offset2 = page.indexOf("{");
        int offset3 = page.lastIndexOf("}");
        page = page.mid(offset2, offset3 + 1);

        QVariantMap creds = QJsonDocument::fromJson(page).toVariant().toMap();

        _makeLoginRequest(username.toUtf8(), password.toUtf8(), creds);
    }
}

void SpotifyIO::loginReply()
{
    qDebug() << "resolver request";
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        QByteArray data(reply->readAll());
        QVariantMap json = QJsonDocument::fromJson(data).toVariant().toMap();

        if (json["status"].toString() == "OK") {
            settings = json["config"].toMap();
            QVariantMap resolver = settings["aps"].toMap()["resolver"].toMap();

            QUrl url;
            url.setScheme("https");
            url.setHost(resolver["hostname"].toString());
            QUrlQuery query;
            query.addQueryItem("client", "24:0:0:" + settings["version"].toString());
            QString site = resolver["site"].toString();
            if (!site.isEmpty()) {
                query.addQueryItem("site", site);
            }
            url.setQuery(query);
            QNetworkRequest req(url);
            req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
            QObject::connect(nam->get(req), SIGNAL(finished()), this, SLOT(resolverReply()));
        }
        else {
            qDebug() << json["error"];
            Q_EMIT authFailed(json["error"].toString());

            _state = DisconnectedState;
            Q_EMIT stateChanged();
        }
    }
}

void SpotifyIO::resolverReply()
{
    qDebug() << "connecting to websocket";
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        QByteArray data(reply->readAll());
        QVariantMap json = QJsonDocument::fromJson(data).toVariant().toMap();
        QString host = json["ap_list"].toList().last().toString().split(":").first();
        host.prepend("wss://");
        host.append("/");
        qDebug() << host;

        QUrl url(host);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
        ws->open(req);
    }
}

void SpotifyIO::onPongReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        QByteArray data = reply->readAll();
        QVariantMap json = QJsonDocument::fromJson(data).toVariant().toMap();
        if (json["status"].toString() == "100") {
            sendCommand("sp/pong_flash2", QVariantList() << json["pong"].toString().split("-").join(" "));
        }
    }
}

void SpotifyIO::onHeartbeat()
{
    sendCommand("sp/echo", QVariantList() << "h", this, COMMANDCALLBACK(onEchoReply));
}

void SpotifyIO::onConnected()
{
    qDebug() << "connected";

    _state = AuthorizingState;
    Q_EMIT stateChanged();

    QStringList creds = settings["credentials"].toList()[0].toString().split(":");
    QVariantList args;
    args << creds[0];
    args << creds[1];
    creds = creds.mid(2);
    args << creds.join(":");
    sendCommand("connect", args);
}

void SpotifyIO::onDisconnected()
{
    qDebug() << "disconnected";

    _state = DisconnectedState;
    Q_EMIT stateChanged();

    heartbeat->stop();
}

void SpotifyIO::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << state;
    switch (state) {
    case QAbstractSocket::ConnectingState: {
        break;
    }
    case QAbstractSocket::ConnectedState: {
        seq = 0;
        break;
    }
    case QAbstractSocket::ClosingState: {
        break;
    }
    case QAbstractSocket::UnconnectedState: {
        break;
    }
    default: {
        break;
    }
    }
}

void SpotifyIO::onTextMessageReceived(const QString &message)
{
    QVariantMap json = QJsonDocument::fromJson(message.toUtf8()).toVariant().toMap();
    if (json.contains("message")) {
        qDebug() << message;
        QStringList args = json["message"].toStringList();
        if (args[0] == "do_work") {
            QScriptEngine engine;
            QScriptValue replyFunction = engine.newFunction(reply);
            engine.globalObject().setProperty("reply", replyFunction);
            QScriptValue value = engine.evaluate(args[1]);
            sendCommand("sp/work_done", QVariantList() << value.toString());
        }
        else if (args[0] == "ping_flash2") {
            QString ping = args[1];
            //QString pong = _constructPong(ping);
            //sendCommand("sp/pong_flash2", QVariantList() << pong);
            ping = ping.split(" ").join("-");
            QUrl url;
            url.setScheme("http");
            url.setHost("ping-pong.spotify.nodestuff.net");
            url.setPath("/" + ping);
            QNetworkRequest req(url);
            req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
            QObject::connect(nam->get(req), SIGNAL(finished()), this, SLOT(onPongReply()));
        }
        else if (args[0] == "login_complete") {
            sendCommand("sp/log", QVariantList() << 41 << 1 << 0 << 0 << 0 << 0);
            sendCommand("sp/user_info", QVariantList(), this, COMMANDCALLBACK(onUserInfo));

            getPlaylists(username, this, COMMANDCALLBACK(onMyPlaylists));

            _state = LoggedInState;
            Q_EMIT stateChanged();

            heartbeat->start();
        }
        else {
            qWarning() << args[0] << "NOT IMPLEMENTED!";
        }
    }
    else if (json.contains("id")) {
        int id = json["id"].toInt();
        if (callbacks.contains(id)) {
            //qDebug() << "have callback for" << id << callbacks[id];
            QMetaObject::invokeMethod(receivers.take(id), callbacks.take(id), Q_ARG(QVariant, json["result"]));
        }
        else {
            qDebug() << message;
        }
    }
}

void SpotifyIO::onBinaryMessageReceived(const QByteArray &message)
{
    qDebug() << message.toHex();
}

void SpotifyIO::onError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}

void SpotifyIO::onSslErrors(const QList<QSslError> &errors)
{
    qDebug() << errors;
}

void SpotifyIO::onUserInfo(const QVariant &data)
{
    userInfo = data.toMap();
    qDebug() << userInfo;
}

void SpotifyIO::onEchoReply(const QVariant &data)
{
    QString result = data.toString();
    if (result != "h") {
        qDebug() << "Something wrong with echo reply";
    }
}

void SpotifyIO::onMyPlaylists(const QVariant &data)
{
    QStringList playlists = data.toStringList();
    foreach (const QString &playlist, playlists) {
        spotify::playlist4::proto::SelectedListContent listContents;
        QByteArray playlistBytes = QByteArray::fromBase64(playlist.toLatin1());
        listContents.ParseFromArray(playlistBytes.constData(), playlistBytes.size());

        ::std::string s_playlistName = listContents.attributes().name();
        QByteArray playlistName(s_playlistName.data(), s_playlistName.length());
        qDebug() << QString::fromUtf8(playlistName) << playlistName.size();
        qDebug() << listContents.contents().items_size() << "items of" << listContents.length();

        for (int i = 0; i < listContents.contents().items_size(); i++) {
            ::std::string s_playlistUri = listContents.contents().items(i).uri();
            QByteArray playlistUri(s_playlistUri.data(), s_playlistUri.length());
            qDebug() << playlistUri;
            QStringList playlistParts = QString::fromLatin1(playlistUri).split(":");
            getPlaylist(playlistParts[2], playlistParts[4], this, COMMANDCALLBACK(onPlaylistContents));
        }
    }
}

void SpotifyIO::onPlaylistContents(const QVariant &data)
{
    QStringList playlists = data.toStringList();
    foreach (const QString &playlist, playlists) {
        spotify::playlist4::proto::SelectedListContent listContents;
        QByteArray playlistBytes = QByteArray::fromBase64(playlist.toLatin1());
        listContents.ParseFromArray(playlistBytes.constData(), playlistBytes.size());

        ::std::string s_playlistName = listContents.attributes().name();
        QByteArray playlistName(s_playlistName.data(), s_playlistName.length());
        qDebug() << QString::fromUtf8(playlistName) << playlistName.size();
        qDebug() << listContents.contents().items_size() << "items of" << listContents.length();

        for (int i = 0; i < listContents.contents().items_size(); i++) {
            ::std::string s_trackUri = listContents.contents().items(i).uri();
            QByteArray trackUri(s_trackUri.data(), s_trackUri.length());
            qDebug() << trackUri;
        }
    }
}

void SpotifyIO::searchReply(const QVariant &data)
{
    Q_EMIT searchCompleted(data.toString());
}

void SpotifyIO::parseMetadataToPlay(const QVariant &data)
{
    parseMetadata(data, this, COMMANDCALLBACK(trackUriPlay));
}

void SpotifyIO::parseMetadataToDownload(const QVariant &data)
{
    parseMetadata(data, this, COMMANDCALLBACK(trackUriDownload));
}

void SpotifyIO::parseMetadata(const QVariant &data, QObject *receiver, const char *callback)
{
    QStringList metadataList = data.toStringList();
    QString metadataHeader = metadataList.first();
    QByteArray byteData = QByteArray::fromBase64(metadataHeader.toLatin1());
    spotify::mercury::proto::MercuryReply mercuryReply;
    mercuryReply.ParseFromArray(byteData.constData(), byteData.size());
    ::std::string replyStatus = mercuryReply.status_message();
    QByteArray replyStatusBytes(replyStatus.data(), replyStatus.length());
    if (replyStatusBytes == "vnd.spotify/mercury-mget-reply") {
        qDebug() << "TODO";
    }
    else {
        parseMetadataItem(replyStatusBytes, QByteArray::fromBase64(metadataList[1].toLatin1()), receiver, callback);
    }
}

void SpotifyIO::parseMetadataItem(const QByteArray &contentType, const QByteArray &data, QObject *receiver, const char *callback)
{
    if (contentType == "vnd.spotify/metadata-album") {
        spotify::metadata::proto::Album album;
        album.ParseFromArray(data.constData(), data.size());
    }
    else if (contentType == "vnd.spotify/metadata-artist") {
        spotify::metadata::proto::Artist artist;
        artist.ParseFromArray(data.constData(), data.size());
    }
    else if (contentType == "vnd.spotify/metadata-track") {
        spotify::metadata::proto::Track track;
        track.ParseFromArray(data.constData(), data.size());
        qDebug() << track.duration();
        pendingTrack["duration"] = track.duration();

        ::std::string trackGid = track.gid();
        QByteArray gid(trackGid.data(), trackGid.length());

        sendCommand("sp/track_uri",
                    QVariantList()
                        << "mp3160"
                        << SpotifyUtils::gid2id(QString::fromLatin1(gid)),
                    receiver,
                    callback);
    }
    else {
        qDebug() << "Unrecognised metadata type:" << contentType;
        return;
    }
}

void SpotifyIO::trackUriPlay(const QVariant &data)
{
    QVariantMap trackData = data.toMap();
    if (trackData.contains("uri")) {
        pendingTrack["lid"] = trackData["lid"];
        if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
            sendTrackEnd();
        }
        currentTrack = pendingTrack;
        mediaPlayer->playSession(trackData["uri"].toString(), trackData["lid"].toString());
    }
}

void SpotifyIO::trackUriDownload(const QVariant &data)
{
    QVariantMap trackData = data.toMap();
    if (trackData.contains("uri")) {
        qDebug() << trackData;
        //TrackDownloader *downloader = new TrackDownloader(userAgent, this);
        //downloader->setData(trackData["uri"].toString(), "/home/nemo/track.mp3");
        //downloader->download();
    }
}

QByteArray SpotifyIO::makeForm(const QVariantMap &formData)
{
    QByteArray data;
    foreach (const QString &key, formData.keys()) {
        if (!data.isEmpty()) {
            data.append("&");
        }
        data.append(key);
        data.append("=");
        data.append(formData[key].toString());
    }
    return data;
}
