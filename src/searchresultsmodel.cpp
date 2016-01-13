#include "searchresultsmodel.h"
#include "spotifyio.h"
#include "spotifyutils.h"

#include <QDebug>

SearchResultsModel::SearchResultsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    _query.clear();
    _xml.clear();
    _maxCount = 0;

    QStringList tracksKeys;
    //searchreply values
    tracksKeys << "id";
    tracksKeys << "type";
    tracksKeys << "title";
    tracksKeys << "artistid";
    tracksKeys << "artist";
    tracksKeys << "album";
    tracksKeys << "albumid";
    tracksKeys << "albumartist";
    tracksKeys << "albumartistid";
    tracksKeys << "year";
    tracksKeys << "tracknumber";
    tracksKeys << "length";
    tracksKeys << "files";
    tracksKeys << "cover";
    tracksKeys << "coversmall";
    tracksKeys << "coverlarge";
    tracksKeys << "popularity";
    tracksKeys << "restrictions";
    tracksKeys << "externalids";
    //trackItem data
    tracksKeys << "restriction";
    tracksKeys << "alternative";
    tracksKeys << "files";

    QStringList albumsKeys;
    albumsKeys << "id";
    albumsKeys << "type";
    albumsKeys << "name";
    albumsKeys << "artistid";
    albumsKeys << "artistname";
    albumsKeys << "albumtype";
    albumsKeys << "cover";
    albumsKeys << "coversmall";
    albumsKeys << "coverlarge";
    albumsKeys << "popularity";
    albumsKeys << "restrictions";
    albumsKeys << "externalids";

    QStringList artistsKeys;
    artistsKeys << "id";
    artistsKeys << "type";
    artistsKeys << "name";
    artistsKeys << "portrait";
    artistsKeys << "popularity";
    artistsKeys << "restrictions";

    QStringList playlistsKeys;
    playlistsKeys << "uri";
    playlistsKeys << "type";
    playlistsKeys << "name";

    _moreKeys << tracksKeys << albumsKeys << artistsKeys << playlistsKeys;

    setSearchType(SearchTracks);
}

QString SearchResultsModel::query() const
{
    return _query;
}

void SearchResultsModel::setQuery(const QString &query)
{
    if (_query != query) {
        _query = query;
        Q_EMIT queryChanged();

        //SpotifyIO::search(_query, _searchType, 100, 0, this, COMMANDCALLBACK(searchReply));
    }
}

QString SearchResultsModel::xml() const
{
    return _xml;
}

void SearchResultsModel::setXml(const QString &xml)
{
    if (_xml != xml) {
        _xml = xml;
        Q_EMIT xmlChanged();

        QString totalKey;
        QString itemKey;
        switch (_searchType) {
        case SearchTracks: {
            totalKey = "total-tracks";
            itemKey = "track";
            break;
        }
        case SearchAlbums: {
            totalKey = "total-albums";
            itemKey = "album";
            break;
        }
        case SearchArtists: {
            totalKey = "total-artists";
            itemKey = "artist";
            break;
        }
        case SearchPlaylists: {
            totalKey = "total-playlists";
            itemKey = "playlist";
            break;
        }
        }

        beginResetModel();

        while (_modelData.size() > 0) {
            TrackItem *track = _modelData.takeLast();
            QObject::disconnect(track, 0, 0, 0);
            track->deleteLater();
        }

        QXmlStreamReader reader(_xml);
        while (!reader.atEnd()) {
            reader.readNext();

            if (reader.tokenType() == QXmlStreamReader::StartElement) {
                if (reader.name() == totalKey) {
                    reader.readNext();
                    _maxCount = reader.text().toInt();
                    Q_EMIT maxCountChanged();
                }
                else if (reader.name() == itemKey) {
                    QVariantMap data = readItem(&reader, itemKey);
                    TrackItem *item = new TrackItem(data, this);
                    _modelData.append(item);
                }
            }
        }

        endResetModel();
    }
}

int SearchResultsModel::searchType() const
{
    return _searchType;
}

void SearchResultsModel::setSearchType(int searchType)
{
    if (_searchType != searchType) {
        _searchType = searchType;
        Q_EMIT searchTypeChanged();

        int role = 0;
        foreach (const QString &rolename, _moreKeys[_searchType]) {
            _roles[role++] = rolename.toLatin1();
        }
    }
}

QVariantMap SearchResultsModel::readItem(QXmlStreamReader *reader, const QString &itemName)
{
    QVariantMap item;
    item["type"] = itemName;
    reader->readNext();
    while (reader->tokenType() != QXmlStreamReader::EndElement && reader->name() != itemName) {
        QString elementName = reader->name().toString();
        reader->readNext();
        if (reader->tokenType() == QXmlStreamReader::Characters) {
            item[elementName] = reader->text().toString();
            reader->readNext();
            reader->readNext();
        }
        else if (reader->tokenType() == QXmlStreamReader::StartElement) {
            QVariantList elements;
            while (reader->tokenType() != QXmlStreamReader::EndElement && reader->name() != elementName) {
                if (reader->tokenType() == QXmlStreamReader::StartElement) {
                    QXmlStreamAttributes attrs = reader->attributes();
                    QVariantMap att;
                    foreach (const QXmlStreamAttribute &attr, attrs) {
                        att[attr.name().toString()] = attr.value().toString();
                    }
                    elements.append(att);
                }
                reader->readNext();
                reader->readNext();
            }
            item[elementName] = elements;
            reader->readNext();
        }
    }
    return item;
}

int SearchResultsModel::maxCount() const
{
    return _maxCount;
}

int SearchResultsModel::rowCount(const QModelIndex &parent) const
{
    return _modelData.size();
}

QVariant SearchResultsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= _modelData.count())
        return QVariant();
    return _modelData[row]->getData(_moreKeys[_searchType][role]);
}

void SearchResultsModel::searchReply(const QVariant &data)
{
    qDebug() << data;
}
