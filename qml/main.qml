import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.spotacular 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component { LoginPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    function stateToString(st) {
        switch (st) {
        case 0:  return "Disconnected"
        case 1:  return "Connecting"
        case 2:  return "Authorization"
        case 3:  return "Logged In"
        default: return "Undefined"
        }
    }

    Connections {
        target: SpotifyIO
        onStateChanged: {
            if (SpotifyIO.state == 3) {
                pageStack.replace(Qt.resolvedUrl("pages/MainPage.qml"))
            }
            else if (SpotifyIO.state == 0) {
                pageStack.pop(0, PageStackAction.Immediate)
                pageStack.push(Qt.resolvedUrl("pages/LoginPage.qml"))
            }
        }
        onAuthFailed: {
            console.log(reason)
        }
    }
}
