import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.spotacular 1.0

Page {
    id: page

    SilicaListView {
        id: view

        anchors.fill: parent

        model: SearchResultsModel {
            id: spotacularModel
            searchType: SearchResultsModel.SearchAll
        }

        delegate: searchResultsComponent

        VerticalScrollDecorator { flickable: view }
    }

    Component {
        id: searchResultsComponent
        BackgroundItem {
            id: delegate
            height: Theme.itemSizeSmall

            Column {
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    width: parent.width
                    text: model.title
                }
            }
        }
    }
}

