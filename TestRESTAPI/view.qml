import QtQuick 2.0

//![0]
ListView {
    width: 200; height: 250

    required model

    delegate: Text {
        required property string color
        required property string textt

        text: "Animal: " + color + ", " + textt
    }
}
//![0]
