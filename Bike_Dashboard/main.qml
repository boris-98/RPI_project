import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtLocation 5.11
import QtPositioning 5.11
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4


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

        if(firstCoord)   // center and zoom map only for the first recieved coordinate
        {
            navigationMap.center = QtPositioning.coordinate(lat, lon)
            navigationMap.zoomLevel = 17
            firstCoord = false
        }
        cyclePath.addCoordinate(QtPositioning.coordinate(lat, lon))

        console.log("addCoordinatesToMap is executed")
        console.log(lat)
    }

    function changeSpeed(speed) {
        crc.value = speed
        distance = distance + 2 * Math.PI * wheelRadius
        watchDog = 0
        if(stopwatchTimer === false)
            stopwatchTimer.running = true

        console.log("changeSpeed is executed")
    }

    Timer {
        id: currentTimeUpdateTimer
        interval: 60000; running: true; repeat: true

        onTriggered: currentTime.text = Qt.formatTime(new Date(),"hh:mm") // update currentTime every minute

    }

    Timer {
        id: tempTimer
        interval: 1000; running: true; repeat: true

        onTriggered: {
            inclineTriangle.requestPaint()
            var temp = accMeasure.getAcc()

            if(accMeasure.getAcc() > 0)
                inclineText.text =  "ascent " + temp + "°"
            else if(accMeasure.getAcc() < 0)
                inclineText.text = "descent " + temp + "°"
            else
                inclineText.text = "   flat"
        }
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

            header: Item {
                height: 35

                Text{
                    id: currentTime
                    text: Qt.formatTime(new Date(),"hh:mm")
                    font.family: "Source Code Pro"
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 50
                    font.letterSpacing: 3
                }
            }

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


            CircularGauge {

                id: crc
                maximumValue: 60
                stepSize: 0.1
                value: 0
                anchors.centerIn: parent
                onValueChanged: canvas.requestPaint()

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
                    onPressAndHold: {hours = 0; minutes = 0; seconds = 0; distance = 0; crc.value = 0; stopwatchTimer.running = false;
                                     cyclePath.path = []; firstCoord = true; bluetoothMain.resetCoords() }
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

                Text {
                    id: labelStopwatch
                    text: qsTr("stopwatch")
                    anchors.horizontalCenter: parent.horizontalCenter
                    y : 60
                }
            }

            Text {
                id: distanceText
                x : 30
                y : 50
                text: ((distance/1000).toFixed(1)).toString() + "km"
                font.pixelSize: 50

                Text {
                    id: labelDistance
                    text: qsTr("distance")
                    anchors.horizontalCenter: parent.horizontalCenter
                    y : 60
                }
            }

            Canvas {
                id: inclineTriangle
                anchors.fill: parent

                onPaint: {
                    var context = getContext("2d")

                    context.clearRect(0, 0, width, height)
                    context.beginPath();

                    if(accMeasure.getAcc() >= 0){
                        context.moveTo(30, height - 50);
                        context.lineTo(180, height - 50);
                        context.lineTo(180, height - 50 - accMeasure.getAcc()*3);
                    }
                    else
                    {
                        context.moveTo(180, height - 50);
                        context.lineTo(30, height - 50);
                        context.lineTo(30, height - 50 + accMeasure.getAcc()*3);
                    }

                    context.closePath();
                    context.lineWidth = 10;
                    context.fillStyle = "red";
                    context.fill();
                }
            }
            Text {
                id: inclineText
                x: 50
                y: parent.height - 50

                font.pixelSize: 25
                font.letterSpacing: 2
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
                        { latitude: 45.27932, longitude: 19.79132 },
                        { latitude: 45.28071, longitude: 19.78356 },
                        { latitude: 45.28130, longitude: 19.77919 },
                        { latitude: 45.28271, longitude: 19.77346 },
                        { latitude: 45.28365, longitude: 19.76861 },
                        { latitude: 45.29103, longitude: 19.74626 },
                        { latitude: 45.28923, longitude: 19.74496 },
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
