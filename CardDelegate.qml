import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
Item {
    width: listview.cellWidth; height: listview.cellHeight
    id: bgitem
    property bool editMode: (model.fullname === "Enter Name") ? true : false
    Item {
        anchors.fill: parent
        Item {
            id: card
            width: 150
            height: 170
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 30
            Rectangle {
                id: profbg
                color: "#aaa7ae"
                anchors.fill: parent
                opacity: 0.4
                radius: 10
            }
            RoundedImage {
                id: img
                width: 45
                height: 45
                anchors.horizontalCenter: profbg.horizontalCenter
                anchors.top: profbg.top
                anchors.topMargin: -7
                source: model.picurl
                asynchronous: true
                Rectangle {
                    visible: bgitem.editMode
                    anchors.fill: parent
                    color: "grey"
                    opacity: imgmousearea.containsMouse ? 0.9 : 0.7
                    Text {anchors.centerIn: parent; text: "Change"; font.pixelSize: 10 }
                }
            }

            Text {
                id: name
                visible: !bgitem.editMode
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: img.bottom
                anchors.topMargin: 10
                font.family: "Noto Serif"
                font.bold: Font.Medium
                font.capitalization: Font.Capitalize
                font.pixelSize: 12
                text: model.fullname
            }

            Text {
                id: owntext
                visible: !bgitem.editMode
                text: model.debt
                font.family: "Noto Serif"
                font.bold: Font.Medium
                font.capitalization: Font.Capitalize
                font.pixelSize: 12
                anchors.top: name.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10

            }



            MyButton {
                id: editbutton
                visible: delegateitemmousearea.containsMouse || bgitem.editMode
                anchors.top: owntext.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 15
                text: bgitem.editMode ? "Save" : "Edit"
                font.family: "Noto Serif"
                bgitem.radius: 20
                bgitem.border.width: 0
                bgitem.implicitWidth: 80
                bgitem.color: !editbuttonarea.containsPress ? "#7a1568" : "#92287f"
                bgitem.opacity: {
                    if(editbuttonarea.containsPress){
                        return 0.5
                    }
                    if(editbuttonarea.containsMouse){
                        return 0.7
                    }
                    return 1
                }
                Behavior on visible {
                    NumberAnimation {
                        target: editbutton
                        property: "opacity"
                        duration: 1000
                        from: 0
                        to: 1.0
                    }
                }

            }

            MouseArea {
                id: delegateitemmousearea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if (!viewbackground.removeMode) {
                        return null;
                    }
                    if (bgitem.state == "selected") {
                        bgitem.state = "notselected"
                        listview.cardIndex.splice(listview.cardIndex.indexOf(index), 1)
                   //     console.log(listview.cardIndex.length)
                        if (listview.cardIndex.length === 0) {
                            viewbackground.removeMode = false
                        }
                        return null;
                    }
                    bgitem.state = "selected"
                    listview.cardIndex.push(index)
//                    console.log(index)
                }

                onPressAndHold: {
                    if (viewbackground.removeMode) {
                        return null;
                    }
                    bgitem.state = "selected"
                    viewbackground.removeMode = true
                    listview.cardIndex.push(index)
//                    console.log(index)
                }

                MouseArea {
                    id: editbuttonarea
                    width: editbutton.width
                    height: editbutton.height
                    x: editbutton.x; y: editbutton.y
                    hoverEnabled: true
                    onClicked: {
                        bgitem.editMode = !editMode
                        // if button pressed in save mode, change values
                        if (!bgitem.editMode && nameEdit.text != "" & owntext.text != "" &
                                (nameEdit.text != name.text || ownedit.value !== owntext.text)) {
                            model.fullname = nameEdit.text
                            model.debt = ownedit.value.toString()
                            modeldb.updateDB(index)
                        }
                    }
                }

                MouseArea {
                    id: imgmousearea
                    width: img.width
                    height: img.height
                    x: img.x; y: img.y
                    hoverEnabled: true
                    onClicked: {
                        if (editMode) {
                            fdialog.makeenable(index)
                        }
                    }
                }

            }

            MyTextInput {
                id: nameEdit
                visible: bgitem.editMode
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: img.bottom
                anchors.topMargin: 10
                height: 20
                font.family: "Noto Serif"
                font.capitalization: Font.Capitalize
                font.pixelSize: 11
                input.clip: true
                text: model.fullname
                KeyNavigation.tab: ownedit
            }

            MySpinBox {
                id: ownedit
                visible: bgitem.editMode
                anchors.top: nameEdit.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 15
//                height: 20
                font.family: "Noto Serif"
//                font.pixelSize: 12
                value: Number(model.debt)
                KeyNavigation.tab: nameEdit
            }
        }
    }

    states: [
        State {
            name: "selected"
            PropertyChanges { target: profbg; color: "#a95569" }
        },
        State {
            name: "notselected"
            PropertyChanges { target: profbg; color: "#aaa7ae" }
        }

    ]
}
