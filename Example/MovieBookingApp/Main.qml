import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Movie Booking App"
    color: "#f0f0f0"

    ListModel { id: movieModel }
    ListModel { id: theaterModel }
    ListModel { id: seatModel }

    property int selectedMovieId: -1
    property int selectedTheaterId: -1

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Movies Section
        Rectangle {
            Layout.preferredWidth: parent.width * 0.3
            Layout.fillHeight: true
            color: "white"; border.color: "#ccc"; radius: 8

            ColumnLayout {
                anchors.fill: parent; spacing: 10;

                Label {
                    text: "Movies"
                    font.bold: true
                    font.pointSize: 18
                }

                Button {
                    text: "Load Movies"
                    onClicked: {
                        apiClient.fetchMovies()
                        movieModel.clear()
                        theaterModel.clear()
                        seatModel.clear()
                        selectedMovieId = -1
                        selectedTheaterId = -1
                    }
                }

                ListView {
                    id: moviesList
                    model: movieModel
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 4

                    delegate: Rectangle {
                        width: parent.width
                        height: 40
                        color: ListView.isCurrentItem ? "#87ceeb" : "#f9f9f9"
                        border.width: 1; border.color: "#ddd"

                        Text {
                            text: title
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                moviesList.currentIndex = index
                                selectedMovieId = id
                                apiClient.fetchTheaters(id)
                                theaterModel.clear()
                                seatModel.clear()
                                selectedTheaterId = -1
                            }
                        }
                    }
                }
            }
        }

        // Theaters Section
        Rectangle {
            Layout.preferredWidth: parent.width * 0.3
            Layout.fillHeight: true
            color: "white"; border.color: "#ccc"; radius: 8

            ColumnLayout {
                anchors.fill: parent; spacing: 10;

                Label {
                    text: "Theaters"
                    font.bold: true
                    font.pointSize: 18
                }

                ListView {
                    id: theatersList
                    model: theaterModel
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 4

                    delegate: Rectangle {
                        width: parent.width
                        height: 40
                        color: ListView.isCurrentItem ? "#add8e6" : "#fafafa"
                        border.width: 1; border.color: "#ddd"

                        Text {
                            text: name
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                theatersList.currentIndex = index
                                selectedTheaterId = id
                                apiClient.fetchSeats(selectedMovieId, id)
                                seatModel.clear()
                            }
                        }
                    }
                }
            }
        }

        // Seats Section
        Rectangle {
            Layout.preferredWidth: parent.width * 0.4
            Layout.fillHeight: true
            color: "white"; border.color: "#ccc"; radius: 8

            ColumnLayout {
                anchors.fill: parent; spacing: 10;

                Label {
                    text: "Seats"
                    font.bold: true
                    font.pointSize: 18
                }

                GridView {
                    id: seatsView
                    model: seatModel
                    cellWidth: 50
                    cellHeight: 50
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                   // spacing: 10

                    delegate: Rectangle {
                        width: 45; height: 45
                        radius: 6
                        color: booked ? "#ffcccb" : "#90ee90" // booked = light red, available = light green
                        border.width: 1; border.color: "#444"
                        property bool selected: false

                        Text {
                            text: seat
                            anchors.centerIn: parent
                            color: booked ? "#777" : "#000"
                        }

                        MouseArea {
                            anchors.fill: parent
                            //enabled: !booked
                            onClicked: {                                
                                if (booked) {
                                    console.log("Canceling seat:", seat)
                                    apiClient.cancelSeat(selectedMovieId, selectedTheaterId, [seat])
                                } else {
                                        console.log("Booking seat:", seat)
                                        apiClient.bookSeat(selectedMovieId, selectedTheaterId, [seat])
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Signals
    Connections {
        target: apiClient

        function onMoviesReceived(movies) {
            movieModel.clear()
            for (let m of movies)
                movieModel.append({ id: m.id, title: m.title })
        }

        function onTheatersReceived(theaters) {
            theaterModel.clear()
            for (let t of theaters)
                theaterModel.append({ id: t.id, name: t.name })
        }

        function onSeatsReceived(seats) {
            seatModel.clear()
            for (let key in seats) {
               // console.log("Seats update received")
                seatModel.append({ seat: key, booked: seats[key] })
            }
        }

        // function onBookingConfirmed(bookingId) {
        //        console.log("Booking successful, ID:", bookingId)
        //        apiClient.fetchSeats(selectedMovieId, selectedTheaterId)
        //    }
    }

    Timer {
        interval: 500
        running: true
        repeat: true
        onTriggered: {
            if (selectedMovieId !== -1 && selectedTheaterId !== -1) {
                apiClient.fetchSeats(selectedMovieId, selectedTheaterId)
            }
        }
    }

}
