import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtLocation 5.11
import QtPositioning 5.11
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

ApplicationWindow {
    visible: true
    width: 700
    height: 700
    title: qsTr("Bike Dashboard")

    function addCoordinatesToMap(a, b){
        cyclePath.addCoordinate(QtPositioning.coordinate(a, b))
        console.log("blabla")
    }

    Plugin {
        id: mapPlugin
        name: "esri"
//               PluginParameter {
//                    name: "osm.mapping.custom.host"
//                    value: "http://a.tile.openstreetmap.fr/hot/"
//                }
//               PluginParameter {
//                           name: "osm.mapping.providersrepository.disabled"
//                           value: "true"
//                       }

        //PluginParameter { name: "osm.mapping.prefetching_style"; value: "NoPrefetching" }
      //  PluginParameter{
      //      mapboxgl.access_token : "pk.eyJ1IjoiYmJvcmlzcyIsImEiOiJja3IzZXE0a3oyaWVtMzFxaDRlZ3N6ZGk0In0.YtKDQAVsJ7U0Qd2_vz2GEA"
      //  }
    }


    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        NavigationPageForm {

            Map {
                anchors.fill: parent
                plugin: mapPlugin
                center: QtPositioning.coordinate(45.26, 19.81)
                zoomLevel: 12
                gesture.enabled: true
                gesture.acceptedGestures: MapGestureArea.PinchGesture | MapGestureArea.PanGesture

                MapPolyline {
                    id: cyclePath
                    line.width: 3
                    line.color: 'green'
                    path: [
                        { latitude: 45.26, longitude: 19.81 },
                        { latitude: 45.5, longitude: 20.0 },
                        { latitude: 46.0, longitude: 21.0 }
                    ]

                }


            }

        }

    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Page 1")
        }
        TabButton {
            text: qsTr("Page 2")
        }
    }

}
