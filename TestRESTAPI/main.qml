import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import io.qt.examples.backend 1.0

Window {
    id: root
    width: 640
    height: 480
    title: qsTr("Hello World")
    visible: true

    BackEnd {
        id: backend
    }

    TextField {
        text: backend.userName
        placeholderText: qsTr("User name")
        anchors.centerIn: parent

        onTextChanged: backend.userName = text
    }

    ListModel {
        id: model
        ListElement {
            name: "Bill Smith"
            number: "555 3264"
        }
        ListElement {
            name: "John Brown"
            number: "555 8426"
        }
        ListElement {
            name: "Sam Wise"
            number: "555 0473"
        }
    }

    ListView {
            id: myListView
            anchors.fill: parent
            //required model1
            delegate: Component {
                Item {
                    width: parent.width
                    height: 40
                    Column {
                        Text {
                            required property string color
                            text: 'color:' + color }
                        Text {
                            required property string text
                            text: 'text:' + text }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: myListView.currentIndex = index
                    }
                }
            }
            highlight: Rectangle {
                color: 'grey'
                Text {
                    anchors.centerIn: parent
                    text: 'Hello ' + model1.get(myListView.currentIndex).text
                    color: 'white'
                }
            }
            focus: true
            onCurrentItemChanged: console.log(model1.get(myListView.currentIndex).text + ' selected')
        }
}
