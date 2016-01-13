#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <sailfishapp.h>

#include "spotifyio.h"
#include "searchresultsmodel.h"

const QString appName("harbour-spotacular");
const QString displayName("Spotacular");
const QString orgName("harbour-spotacular");

static QObject *spotifyio_singleton(QQmlEngine *, QJSEngine *)
{
    return SpotifyIO::GetInstance(0);
}

int main(int argc, char *argv[])
{
    qmlRegisterSingletonType<SpotifyIO>("harbour.spotacular", 1, 0, "SpotifyIO", spotifyio_singleton);
    qmlRegisterType<SearchResultsModel>("harbour.spotacular", 1, 0, "SearchResultsModel");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationName(appName);
    app->setApplicationDisplayName(displayName);
    app->setOrganizationName(orgName);

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setTitle(displayName);
    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->show();

    return app->exec();
}
