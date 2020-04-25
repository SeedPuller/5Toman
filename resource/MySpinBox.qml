import QtQuick 2.0
import QtQuick.Controls 2.5
SpinBox {
    id: root
    width: 80
    height: 20
    stepSize: 5000
    from: 0
    to: 100000000
    value: Number("000")
    background: Rectangle {
        id: spinbg
        implicitWidth: 100
        implicitHeight: 40
        radius: 10
    }
    contentItem: Rectangle{
        anchors.centerIn: spinbg
        width: 120
        height: 20
        Text {
            anchors.centerIn: parent
            text: root.value
        }
    }

    up.indicator: Rectangle {
        anchors.right: spinbg.right
        anchors.rightMargin: -10
        implicitHeight: 20
        implicitWidth: 20
        radius: 10
        color: upindicator.containsMouse ? "#92287f" : "#7a1568"
        Text {
            anchors.centerIn: parent
            text: "+"
            font.pixelSize: 15
        }
        MouseArea {
            id: upindicator
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.increase()
        }
    }
    down.indicator: Rectangle {
        anchors.left: spinbg.left
        anchors.leftMargin: -10
        implicitHeight: 20
        implicitWidth: 20
        radius: 10
        color: {
            if (!enabled) {
                return "#b0389b"
            }
            return downindicator.containsMouse ? "#92287f" : "#7a1568"
        }
        Text {
            anchors.centerIn: parent
            text: "-"
            font.pixelSize: 15
            opacity: enabled ? 1.0 : 0.6
        }
        MouseArea {
            id: downindicator
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.decrease()
        }
    }
}
