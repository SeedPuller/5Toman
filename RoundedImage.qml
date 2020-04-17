import QtQuick 2.0
import QtGraphicalEffects 1.14

Image {
    id: root
    source: 'file://'
    property bool rounded: true
    property bool adapt: true
    height: 40
    width:  40
    layer.enabled: rounded
    layer.effect: OpacityMask {
        maskSource: Item {
            width: root.width
            height: root.height
            Rectangle {
                anchors.centerIn: parent
                width: root.adapt ? root.width : Math.min(img.width, img.height)
                height: root.adapt ? root.height : width
                radius: Math.min(width, height)
            }
        }
    }
}
