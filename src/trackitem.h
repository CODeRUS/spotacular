#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <QObject>

#include "mercury.pb.h"
#include "metadata.pb.h"

#include "spotifyio.h"
#include "spotifyutils.h"

class TrackItem : public QObject
{
    Q_OBJECT
public:
    explicit TrackItem(const QVariantMap &data, QObject *parent = 0);
    QVariant getData(const QString &key);

private:
    QVariantMap _data;

signals:

public slots:

private slots:
    void onMetadataReceived(const QVariant &data);
    void onTrackUriReceived(const QVariant &data);

};

#endif // TRACKITEM_H
