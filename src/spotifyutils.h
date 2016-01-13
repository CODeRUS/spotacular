#ifndef SPOTIFYUTILS_H
#define SPOTIFYUTILS_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

class SpotifyUtils : public QObject
{
    Q_OBJECT
public:
    static QString uriType(const QString &uri);
    static QString gid2id(const QString &gid);
    static QString id2uri(const QString &uritype, const QString &id);
    static QString uri2id(const QString &uri);
    static QString gid2uri(const QString &uritype, const QString &gid);

    static QList<int> func_g(QList<int> a, int b, int c);
    static QList<int> func_f(QString a, QString b);
    static QStringList func_h(QList<int> a, QString b);

};

#endif // SPOTIFYUTILS_H
