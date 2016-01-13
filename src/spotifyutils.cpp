#include "spotifyutils.h"

#include <QDebug>

const QString base62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const QString base16 = "0123456789abcdef";

QString SpotifyUtils::uriType(const QString &uri)
{
    QStringList parts = uri.split(":");
    if (parts.size() >= 3 && parts[1] == "local")
        return "local";
    else if (parts.size() >= 5)
        return parts[3];
    else if (parts.size() >= 4 && parts[3] == "starred")
        return "playlist";
    else if (parts.size() >= 3)
        return parts[1];
    else
        return QString();
}

QString SpotifyUtils::gid2id(const QString &gid)
{
    return QString::fromLatin1(gid.rightJustified(16, QLatin1Char('\0')).toLatin1().toHex());
}

QString SpotifyUtils::id2uri(const QString &uritype, const QString &id)
{
    QList<int> tmp1 = SpotifyUtils::func_f(id.toLower(), base16);
    QList<int> tmp2 = SpotifyUtils::func_g(tmp1, 16, 62);
    QStringList res = SpotifyUtils::func_h(tmp2, base62);
    return QString("spotify:%1:%2").arg(uritype).arg(res.join(""));
}

QString SpotifyUtils::uri2id(const QString &uri)
{
    QStringList parts = uri.split(":");
    QString s;
    if (parts.size() > 3 && parts[3] == "playlist")
        s = parts[4];
    else
        s = parts[2];

    int v = 0;
    foreach (const QChar &c, s)
        v = v * 62 + base62.indexOf(c);
    return QString::number(v, 16).mid(2).rightJustified(32, QLatin1Char('0'));
}

QString SpotifyUtils::gid2uri(const QString &uritype, const QString &gid)
{
    QString id = SpotifyUtils::gid2id(gid);
    QString uri = SpotifyUtils::id2uri(uritype, id);
    return uri;
}

QList<int> SpotifyUtils::func_g(QList<int> a, int b, int c)
{
    QList<int> d;
    d.append(0);
    QList<int> f;
    f.append(1);
    for (int g = 0; g < a.length(); ++g) {
        int q = a[g];
        int t = 0;
        int y = 0;
        for (y = 0; y < f.length(); ++y) {
            if (y == d.length()) {
                t = f[y] * q + t;
                d.append(t % c);
            }
            else {
                t = ~~d[y] + f[y] * q + t;
                d[y] = t % c;
            }
            t = ~~ (t / c);
        }
        for (; t;) {
            if (y == d.length()) {
                d.append(t % c);
            }
            else {
                t = ~~d[y] + t;
                d[y] = t % c;
            }
            t = ~~ (t / c);
            ++y;
        }
        int u = 0;
        for (int m = 0; m < f.length(); ++m) {
            u = f[m] * b + u;
            f[m] = u % c;
            u = ~~ (u / c);
        }
        for (; u;) {
            f.append(u % c);
            u = ~~ (u / c);
        }
    }
    return d;
}

QList<int> SpotifyUtils::func_f(QString a, QString b)
{
    QList<int> d;
    for (int c = 0; c < a.length(); ++c) {
        d.prepend(b.indexOf(a.mid(c, 1)));
    }
    return d;
}

QStringList SpotifyUtils::func_h(QList<int> a, QString b)
{
    QStringList d;
    for (int c = 0; c < a.length(); ++c) {
        d.prepend(b.mid(a[c], 1));
    }
    return d;
}
