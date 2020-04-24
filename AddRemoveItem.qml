import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    id: additem
    Layout.fillWidth: true
    Layout.fillHeight: true
    state: viewbackground.removeMode ? "removeMode" : "addMode"
    opacity: {
        if(addmousearea.containsMouse) {
            return 0.85
        }
        return 1
    }

    MouseArea {
        id: addmousearea
        anchors.fill: additem
        hoverEnabled: true
        onClicked: {
            if (viewbackground.removeMode) {
                cardview.cardIndex.sort()
                for (var i = 0; i < cardview.cardIndex.length; ++i) {
//                    console.log("removing number " + cardview.cardIndex[i])
//                    console.log("removing number " + (cardview.cardIndex[i] - i))
                    modeldb.remove(cardview.cardIndex[i] - i)

                }
                cardview.cardIndex = []
                viewbackground.removeMode = false
            } else {
                modeldb.insertFirst("Enter Name", "000", defaultpicurl)
            }
        }
    }

    Rectangle {
        id: additemcolor
        anchors.fill: additem
        color: "#aaa7ae"
        opacity: 0.4
        radius: 10
    }

    Rectangle {
        id: plusminusbg
        anchors.centerIn: parent
        width: 50
        height: 50
   //     font.bold: Font.Black
        radius: 10
        color: "#aaa7ae"
        opacity: 0.6
        Text {
            id: plusminustext
            anchors.fill: parent
            anchors.topMargin: 7
            anchors.leftMargin: 13.5
            text: "\u002b"
            color: "#5e5e5e"
            font.pixelSize: 40
            //anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    states: [
        State {
            name: "removeMode"
            PropertyChanges { target: additemcolor; color: "#a95569" }
            PropertyChanges { target: plusminusbg; color: "#a95569" }
            PropertyChanges { target: plusminustext; text: "\u2212" }
        },

        State {
            name: "addMode"
            PropertyChanges { target: additemcolor; color: "#aaa7ae" }
            PropertyChanges { target: plusminusbg; color: "#aaa7ae" }
            PropertyChanges { target: plusminustext; text: "\u002b" }
        }
    ]

    transitions: [
        Transition {
            from: "addMode"
            to: "removeMode"
            ColorAnimation { target: additemcolor; duration: 400 }
            ColorAnimation { target: plusminusbg; duration: 300 }
            PropertyAnimation { target: plusminustext; duration: 200 }
        },
        Transition {
            from: "removeMode"
            to: "addMode"
            ColorAnimation { target: additemcolor; duration: 400 }
            ColorAnimation { target: plusminusbg; duration: 300 }
            PropertyAnimation { target: plusminustext; duration: 200 }
        }

    ]
}
