import QtQuick 2.4

FocusScope {
    property alias text: __input.text
    property alias font: __input.font
    property alias input: __input
    property alias bgitem: __background
    Behavior on visible {
        NumberAnimation {
            target: __background
            property: "opacity"
            duration: 500
            from: 0
            to: 1
        }
    }
    onFocusChanged: {
        if (focus) {
            __input.selectAll()
        }
    }

    Rectangle {
        id: __background
        anchors.centerIn: parent
        radius: 20
        width: 100
        height: 25
        color: "white"
        border.color: __input.activeFocus ? "grey" : "transparent"
        border.width: __input.activeFocus ? 1 : 0
        TextInput {
            id: __input
            anchors.centerIn: parent
            width: __background.width - 20
            height: __background.height - 10
            color: "black"
            focus: true
            //placeholderText: qsTr("Enter description")
        }
    }
}
