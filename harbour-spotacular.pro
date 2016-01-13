TARGET = harbour-spotacular

QT += network script xml multimedia websockets
CONFIG += sailfishapp

LIBS += /usr/lib/libprotobuf.a

SOURCES += \
    src/main.cpp \
    src/spotifyio.cpp \
    src/spotifyutils.cpp \
    src/searchresultsmodel.cpp \
    src/bartender.pb.cc \
    src/mercury.pb.cc \
    src/metadata.pb.cc \
    src/playlist4changes.pb.cc \
    src/playlist4content.pb.cc \
    src/playlist4issues.pb.cc \
    src/playlist4meta.pb.cc \
    src/playlist4ops.pb.cc \
    src/playlist4service.pb.cc \
    src/pubsub.pb.cc \
    src/toplist.pb.cc \
    src/trackdownloader.cpp \
    src/mediaplayer.cpp \
    src/trackitem.cpp

HEADERS += \
    src/spotifyio.h \
    src/spotifyutils.h \
    src/searchresultsmodel.h \
    src/bartender.pb.h \
    src/mercury.pb.h \
    src/metadata.pb.h \
    src/playlist4changes.pb.h \
    src/playlist4content.pb.h \
    src/playlist4issues.pb.h \
    src/playlist4meta.pb.h \
    src/playlist4ops.pb.h \
    src/playlist4service.pb.h \
    src/pubsub.pb.h \
    src/toplist.pb.h \
    src/trackdownloader.h \
    src/mediaplayer.h \
    src/trackitem.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    rpm/harbour-spotacular.spec \
    harbour-spotacular.desktop \
    qml/main.qml

DISTFILES += \
    qml/pages/LoginPage.qml \
    qml/pages/MainPage.qml \
    qml/components/IconListItem.qml \
    qml/pages/SearchPage.qml
