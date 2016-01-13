import QtQuick 2.1
import Sailfish.Silica 1.0
import harbour.spotacular 1.0

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
                text: qsTr("Spotacular")
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
            TextField {
                id: usernameField
                width: parent.width
                placeholderText: qsTr("Username")
                label: qsTr("Username")
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                focus: true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: passwordField.forceActiveFocus()
            }
            TextField {
                id: passwordField
                width: parent.width
                placeholderText: qsTr("Password")
                label: qsTr("Password")
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: passwordField.focus = false
            }

            Button {
                text: qsTr("Login")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: SpotifyIO.login(usernameField.text, passwordField.text)
                enabled: SpotifyIO.state == 0 && usernameField.text.length > 0 && passwordField.text.length > 0
            }
            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("We're not storing your Spotify credentials, you need to enter your username and password each time before using this application")
                wrapMode: Text.Wrap
            }
        }

        VerticalScrollDecorator {}
    }
}
