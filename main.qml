import QtQuick 2.14
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0
import Model.ViewModel 1.0

ApplicationWindow {
    visible: true
    minimumHeight: 580
    minimumWidth: 860
    width: 1040
    height: 650
    title: "5 Toman"
    font.family: "Noto Serif"
    property var defaultpicurl: "pic/default-profile-pic.png"
    LinearGradient {
        id: mainwindow
        anchors.fill: parent //parent.width / 1.16
        start: Qt.point(parent.width / 1.08 , parent.height / 1.6)
        end: Qt.point(parent.width, parent.height)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#162857" }
            GradientStop { position: 1.1; color: "#b068bb" }
        }
    }

    Rectangle {
        id: rectheader
        anchors.top: mainwindow.top
        anchors.left: mainwindow.left
        anchors.right: mainwindow.right
        height: 90
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#3962cb" }
            GradientStop { position: 0.8; color: "#162857" }
        }
        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 40
            font.bold: Font.DemiBold
            font.pixelSize: 30
            text: "5 TOMAN"
        }
    }

    Item {
      //  color: "transparent"
        id: viewbackground
        anchors.top: rectheader.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: mainwindow.horizontalCenter
        anchors.left: mainwindow.left
        anchors.leftMargin: 100
        width: mainwindow.width - (mainwindow.width / 4)
        height: mainwindow.height - (mainwindow.height / 3)
        property bool removeMode: false
        RowLayout {
            spacing: 10
            anchors.fill: parent
            Item {
                id: viewbgitem
                //Layout.preferredWidth: 650
                Layout.fillWidth: true
                Layout.fillHeight: true
                Rectangle {
                    id: viewrect
                    anchors.fill: viewbgitem
                    color: "#aaa7ae"
                    opacity: 0.4
                    radius: 10
                }

                CardView { id: cardview }
                Text {
                    visible: (cardview.count < 1)
                    anchors.centerIn: viewbgitem
                    text: "Congrats! There is no debtors!"
                    color: "#aaa7ae"
                }
            }

            ColumnLayout {
                width: 160
                Layout.fillWidth: false
                Layout.fillHeight: true
                AddRemoveItem { }
            }
        }
    }

    ViewModel { id: modeldb }
    FileDialog {
        id: fdialog
        title: "choose file"
        folder: shortcuts.home
        property var index: 0
        function makeenable(indexnum) {
            index = Number(indexnum)
            fdialog.visible = true
        }
        onAccepted: {
             modeldb.setPic(index, (fdialog.fileUrl).toString())
        }
    }
}
