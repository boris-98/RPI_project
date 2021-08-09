import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtLocation 5.11
import QtPositioning 5.11
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtGraphicalEffects 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: qsTr("Bike Dashboard")

    property int watchDog : 0
    property bool firstCoord : true
    property int seconds : 0
    property int minutes : 0
    property int hours : 0
    property real wheelRadius : 0.3
    property double distance : 0

    function addCoordinatesToMap(lat, lon) {
        cyclePath.addCoordinate(QtPositioning.coordinate(lat, lon))

        if(firstCoord)   // center and zoom map only for the first recieved coordinate
        {
            navigationMap.center = QtPositioning.coordinate(lat, lon)
            navigationMap.zoomLevel = 16
            firstCoord = false
        }
        console.log("addCoordinatesToMap is executed")
        console.log(lat)
    }

    function changeSpeed(speed) {
        crc.value = speed
        distance = distance + 2 * Math.PI * wheelRadius
        watchDog = 0
        stopwatchTimer.running = true

        console.log("changeSpeed is executed")
    }

    Timer {
        id : stopwatchTimer
        interval: 1000; running: false; repeat: true

        onTriggered: {
            watchDog = watchDog + 1;
            if(watchDog == 10)   // basically not moving
                crc.value = 0

            if(crc.value > 1) {
                if(seconds < 59) {
                    seconds = seconds + 1;
                }
                else {
                    seconds = 0;

                    if(minutes < 59) {
                        minutes = minutes + 1;
                    }
                    else {
                        minutes = 0;

                        if(hours < 12) {
                            hours = hours + 1;
                        }
                        else {
                            hours = 0;
                        }
                    }
                }
            }
        }
    }

    Plugin {
        id: mapPlugin
        name: "esri"

    }


    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive : false

        SpeedometerPageForm {

            Canvas {
                    id: canvas
                    anchors.fill: parent
                    anchors.centerIn: parent

                    rotation: -235

                    onPaint: {
                        var c = getContext('2d')
                        c.clearRect(0, 0, width, height)
                        c.beginPath()
                        c.lineWidth = 20
                        c.strokeStyle = "red"
                        c.arc(width / 2, height / 2, 170 , 0, 5.06145483 * crc.value/crc.maximumValue)    // 290 degrees = 5.06...rad
                        c.stroke()
                        c.globalAlpha = 0.75
                    }
                }

//                ConicalGradient {
//                    id: progress

//                    rotation: -235
//                        anchors.fill: bar
//                        angle: crc.value
//                        gradient: Gradient {
//                            GradientStop { position: 0.0; color: "transparent" }
//                            GradientStop { position: 0.1; color: "white" }
//                        }
//                         visible: false // Not visible (it will be painted by the mask)
//                }
//                OpacityMask {
//                        anchors.fill: progress
//                        rotation: -235
//                        source: bar
//                        maskSource: progress
//                        invert: true
//                    }

//                Slider {
//                    id: circ
//                    value : 0.5
//                    onValueChanged: canvas.requestPaint()
//                }


            CircularGauge {

                id: crc
                maximumValue: 60
                stepSize: 0.1
                value: 0//accelerating ? maximumValue : 0
                anchors.centerIn: parent
                onValueChanged: canvas.requestPaint()

                property bool accelerating: false

//                Keys.onSpacePressed: accelerating = true
//                Keys.onReleased: {
//                    if (event.key === Qt.Key_Space) {
//                        accelerating = false;
//                        event.accepted = true;
//                    }
//                }

//                Component.onCompleted: forceActiveFocus()

                Behavior on value {
                    NumberAnimation {
                        duration: 1000
                    }
                }

                style : CircularGaugeStyle{

                    tickmarkLabel:  Text {
                        font.pixelSize: styleData.value >= crc.value ? Math.max(6, outerRadius * 0.11) : Math.max(6, outerRadius * 0.15)
                        font.bold : styleData.value >= crc.value ? false : true
                        text: styleData.value
                        color: styleData.value >= crc.value ? "grey" : "black"
                        antialiasing: true

                    }

                    tickmark: Rectangle {
                        implicitWidth: styleData.value >= crc.value ? outerRadius * 0.02 : outerRadius * 0.03
                        antialiasing: true
                        implicitHeight: styleData.value >= crc.value ? outerRadius * 0.06 : outerRadius * 0.08
                        color: styleData.value >= crc.value ? "grey" : "black"
                    }

                    minorTickmark: Rectangle {
                        visible: styleData.value < 80
                        implicitWidth: outerRadius * 0.01
                        antialiasing: true
                        implicitHeight: outerRadius * 0.03
                        color: styleData.value >= crc.value ? "grey" : "black"
                    }

                }

                Text {
                    id: lcdnumber
                    x: crc.value >= 10 ? 125 : 140  // shift for 2 digit numbers to center
                    y: 250
                    text: crc.value.toLocaleString(Qt.locale(), 'f', 1)
                    elide: Text.ElideLeft
                    font.pixelSize: 50
                }

                Button {
                    id : resetButton
                    text: "Reset"
                    onPressAndHold: {hours = 0; minutes = 0; seconds = 0; distance = 0; crc.value = 0; stopwatchTimer.running = false; }
                    x : 125
                    y : 325
                }

            }

            Text {
                id: stopwatchText
                x : parent.width - 180
                y : 50
                text: hours.toString() + ":" + minutes.toString() + ":" + seconds.toString()
                font.pixelSize: 50
            }

            Text {
                x : 30
                y : 50
                id: distanceText
                text: ((distance/1000).toFixed(1)).toString() + "km"
                font.pixelSize: 50
            }

        }

        NavigationPageForm {

            Map {
                id: navigationMap
                anchors.fill: parent
                plugin: mapPlugin
                center: QtPositioning.coordinate(45.26, 19.81)
                zoomLevel: 12
                gesture.enabled: true
                gesture.acceptedGestures: MapGestureArea.PinchGesture | MapGestureArea.PanGesture

                MapPolyline {
                    id: cyclePath
                    line.width: 4
                    line.color: 'blue'
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
            text: qsTr("Instrumentation")
        }
        TabButton {
            text: qsTr("Navigation")
        }
    }

}
