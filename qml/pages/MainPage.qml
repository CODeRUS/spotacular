import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.spotacular 1.0
import "../components"

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            Label {
                x: Theme.paddingLarge
                text: "Spotacular"
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            IconListItem {
                title: qsTr("Search")
                iconSource: "image://theme/icon-m-search"
            }

            IconListItem {
                title: qsTr("Playlists")
                iconSource: "image://theme/icon-m-events"
            }

            IconListItem {
                title: qsTr("Songs")
                iconSource: "image://theme/icon-m-music"
            }

            IconListItem {
                title: qsTr("Albums")
                iconSource: "image://theme/icon-m-levels"
            }

            IconListItem {
                title: qsTr("Artists")
                iconSource: "image://theme/icon-m-people"
            }
        }
    }
}
