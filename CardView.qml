import QtQuick 2.0

GridView {
    id: listview
    property var cardIndex: []
    anchors.fill: viewrect
    anchors.topMargin: 10
    anchors.leftMargin: 50
//    model: newmodel
    model: modeldb
    cellHeight: 200
    cellWidth: 180
    clip: true
    delegate: CardDelegate { }
    add: Transition {
        NumberAnimation {
            property: "x"
            from: -200
            duration: 500
        }
    }

    displaced: Transition {
        NumberAnimation {
            properties: "x,y"
            duration: 300
        }
    }

    remove: Transition {
        ParallelAnimation {
            NumberAnimation {
                property: "y"
                to: 0
                duration: 200
            }

            NumberAnimation {
                property: "x"
                to: -100
                duration: 200
            }
        }
    }
}

