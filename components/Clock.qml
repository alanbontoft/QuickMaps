import QtQuick
import QtQuick.Controls

Rectangle {

    property string time: "00:00:00"

    height: 50

    color: 'black'
    Label {
        font.pixelSize: 40
        anchors.centerIn: parent
        color: 'white'
        text: time
    }
}
