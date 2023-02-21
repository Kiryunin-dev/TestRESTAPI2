import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import io.qt.examples.backend 1.0

Window {
    id: root
    width: 640
    height: 480
    title: qsTr("JSON REST API Клиент")
    visible: true

    BackEnd {
        id: backend
    }

//    TextField {
//        text: backend.userName
//        placeholderText: qsTr("User name")
//        anchors.centerIn: parent

//        onTextChanged: backend.userName = text
//    }

//    ListModel {
//        id: model1
//        ListElement {
//            name: "Bill Smith"
//            number: "555 3264"
//        }
//        ListElement {
//            name: "John Brown"
//            number: "555 8426"
//        }
//        ListElement {
//            name: "Sam Wise"
//            number: "555 0473"
//        }
//    }

//    ListView {
//        width: 200; height: 250
//        anchors.fill: parent
//        model: mymodel

//        delegate: Text {
//            //required property string color
//            //required property string textt

//            text: "Animal: " + colorr + ", " + textt
//        }
//    }

    ListView {
            id: myListView
            anchors.fill: parent
            model: mymodel
            delegate: Component {
                Item {
                    width: 5000
                    height: 30
                    Column {
//                        Text {
//                            text: 'Дата:' + date }
                        Text {
                            text: 'В этот день:' + textt }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                            myListView.currentIndex = index;
                        }
                        onDoubleClicked: textWindow.open(textt);
                    }
                }
            }
            highlight: Rectangle {
                color: 'orange'
                Text {
                    anchors.centerIn: parent
                    text: 'Hello '// + mymodel.get(myListView.currentIndex).text
                    color: 'white'
                }
            }
            focus: true
            //onCurrentItemChanged: console.log(mymodel.get(myListView.currentIndex).text + ' selected')
        }

    Window{
            id: textWindow
            title: qsTr("Описание")
            width: 540
            height: 180
            visible: false

            Label {
                id: txtField
                text: "No data"
                anchors.centerIn: parent
                anchors.fill: parent
                wrapMode: Label.WordWrap
                x: 20
                y: 20
            }

            function open(textData){
                visible = true;
                txtField.text = textData;
            }
            function close(){
                visible = false
            }
        }
}
