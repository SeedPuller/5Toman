import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Button {
    id: root
    text: "Button"
    font.capitalization: Font.MixedCase
    property alias bgitem: __background
//    property alias bgborder: __background.border
    property alias contentText: __text
    property alias area: __mousearea
    signal clicked()
    contentItem: Text {
        id: __text
        text: root.text
        font: root.font
        color: "#f5f5f5"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        opacity: {
            if(__mousearea.containsPress){
                return 0.5
            }
            if(__mousearea.containsMouse){
                return 0.7
            }
            return 1
        }
    }

    background: Rectangle {
        id: __background
        implicitHeight: 30
        implicitWidth: 50
        color: __mousearea.containsPress ? Qt.darker("#3fc1c9") : "#3fc1c9"
        opacity: !root.enabled ? 0.3 : 1
        MouseArea {
            id: __mousearea
            anchors.fill: parent
           // hoverEnabled: true
       //     preventStealing: true
            onEntered: {
                console.log("entered")
                __background.opacity = 0.7
            }

            onExited: __background.opacity = 1
            onPressed: __background.opacity = 0.5
            onReleased: __background.opacity = 1.0
            onClicked: {
                console.log("ok")
                root.clicked()
            }
        }
        radius: 3
        border.width: 0.5
        border.color: Qt.lighter(color)
    }
}
