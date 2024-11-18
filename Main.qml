import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

import QuickMaps

import "components"

Window {
    id: mainWin
    width: 640
    height: 480
    visible: true
    title: qsTr("Quick Map")

    DataModel {
        id: model
    }

    function updateCoordinates(latitude, longitude)
    {
        console.log(latitude, longitude)
        map.center = QtPositioning.coordinate(latitude, longitude)
    }

    Item {

        id: mainItem
        anchors.fill: parent

        Component.onCompleted: {
            model.coordsReceived.connect(updateCoordinates)
        }

        Clock {
            id: clock
            anchors{
                left: mainItem.left
                right: mainItem.right
                top: mainItem.top
            }

            time: model.time
        }

        Plugin {
            id: mapPlugin
            name: "osm"

            PluginParameter {
                name: "osm.mapping.custom.host"
                value: "https://tile.thunderforest.com/landscape/%z/%x/%y.png?apikey=09d4446b42aa4295a5b277607017c08a"
            }
        }



        Map {
            id: map
            anchors {
                left: mainItem.left
                right: mainItem.right
                top: clock.bottom
                bottom: mainItem.bottom
            }

            plugin: mapPlugin
            center: QtPositioning.coordinate(51.5080, -0.128150)  // trafalgar square
            zoomLevel: 16
            activeMapType: map.supportedMapTypes[map.supportedMapTypes.length - 1]

            DragHandler {
                id: drag
                target: null
                onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
            }

            WheelHandler {
                id: wheel
                // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
                // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
                // and we don't yet distinguish mice and trackpads on Wayland either
                acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                                 ? PointerDevice.Mouse | PointerDevice.TouchPad
                                 : PointerDevice.Mouse
                rotationScale: 1/120
                property: "zoomLevel"
            }
        }
    }

}
