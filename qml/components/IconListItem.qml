import QtQuick 2.1
import Sailfish.Silica 1.0

ListItem {
    id: root

    property alias iconSource: icon.source
    property alias title: label.text

    contentHeight: Theme.itemSizeMedium

    Image {
        id: icon
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingLarge
        anchors.verticalCenter: parent.verticalCenter
        fillMode: Image.PreserveAspectFit
        smooth: true
        cache: true
        layer.effect: ShaderEffect {
            property color color: Theme.highlightColor

            fragmentShader: "
                varying mediump vec2 qt_TexCoord0;
                uniform highp float qt_Opacity;
                uniform lowp sampler2D source;
                uniform highp vec4 color;
                void main() {
                    highp vec4 pixelColor = texture2D(source, qt_TexCoord0);
                    gl_FragColor = vec4(mix(pixelColor.rgb/max(pixelColor.a, 0.00390625), color.rgb/max(color.a, 0.00390625), color.a) * pixelColor.a, pixelColor.a) * qt_Opacity;
                }
            "
        }
        layer.enabled: root.highlighted
        layer.samplerName: "source"
    }

    Label {
        id: label

        anchors.left: icon.right
        anchors.right: parent.right
        anchors.margins: Theme.paddingLarge
        anchors.verticalCenter: parent.verticalCenter

        color: root.highlighted ? Theme.highlightColor : Theme.primaryColor
        truncationMode: TruncationMode.Fade
    }
}

