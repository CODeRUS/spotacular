#ifndef SEARCHRESULTSMODEL_H
#define SEARCHRESULTSMODEL_H

#include <QAbstractListModel>
#include <QXmlStreamReader>

#include "trackitem.h"

class SearchResultsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(SearchType)

public:
    explicit SearchResultsModel(QObject *parent = 0);

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    QString query() const;
    void setQuery(const QString &query);

    Q_PROPERTY(QString xml READ xml WRITE setXml NOTIFY xmlChanged)
    QString xml() const;
    void setXml(const QString &xml);

    enum SearchType {
        SearchTracks = 1,
        SearchAlbums = 2,
        SearchArtists = 4,
        SearchPlaylists = 8,
        SearchAll = 15,
    };

    Q_PROPERTY(int searchType READ searchType WRITE setSearchType NOTIFY searchTypeChanged)
    int searchType() const;
    void setSearchType(int searchType);

    Q_PROPERTY (int maxCount READ maxCount NOTIFY maxCountChanged)
    int maxCount() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const { return _roles; }

private slots:
    void searchReply(const QVariant &data);

private:
    QVariantMap readItem(QXmlStreamReader *reader, const QString &itemName);

    QString _query;
    QString _xml;
    int _searchType;
    int _maxCount;

    QList<TrackItem*> _modelData;
    QHash<int, QByteArray> _roles;
    QList<QStringList> _moreKeys;

signals:
    void queryChanged();
    void xmlChanged();
    void searchTypeChanged();
    void maxCountChanged();

};

#endif // SEARCHRESULTSMODEL_H
