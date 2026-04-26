import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: guestWindow
    width: Screen.width
    height: Screen.height
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint | Qt.Window
    title: "Парикмахер - " + Database.currentBarberName

    property int currentTab: 0
    property var loginWindowRef: null

    readonly property string statusPending: "pending"
    readonly property string statusActive: "active"
    readonly property string statusCompleted: "completed"
    readonly property string statusCancelled: "cancelled"

    function refreshData() {
        barberInfo = Database.getBarberInfo()
        activeTasksList.model = Database.getBarberActiveTasks()
        historyList.model = Database.getBarberHistory()
    }

    function showStatus(message) {
        let isError = message.toLowerCase().indexOf("ошибка") !== -1
        notifyRect.color = isError ? "#e94560" : "#0f3460"
        notifyText.text = message
        notifyAnim.restart()
    }

    property var barberInfo: ({})

    Connections {
        target: Database
        function onLogoutSuccess() {
            if (guestWindow && guestWindow.visible) {
                try {
                    if (loginWindowRef) {
                        loginWindowRef.show()
                        guestWindow.destroy()
                    }
                } catch(e) {}
            }
        }
    }

    Component.onCompleted: refreshData()

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#f5f5f5" }
            GradientStop { position: 0.5; color: "#e8e8e8" }
            GradientStop { position: 1.0; color: "#d0d0d0" }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 16

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 64
                radius: 12
                color: "#ffffff"
                border.color: "#0f3460"
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    Text {
                        text: Database.currentBarberName
                        font.pixelSize: 24
                        font.bold: true
                        color: "#1a1a2e"
                        font.family: "Segoe UI"
                    }

                    Item { Layout.fillWidth: true }

                    Rectangle {
                        width: 120
                        height: 40
                        radius: 8
                        color: "#1a1a2e"

                        Text {
                            anchors.centerIn: parent
                            text: "Обновить"
                            color: "white"
                            font.pixelSize: 13
                            font.bold: true
                            font.family: "Segoe UI"
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onEntered: parent.opacity = 0.9
                            onExited: parent.opacity = 1.0
                            onClicked: refreshData()
                        }
                    }

                    Rectangle {
                        width: 100
                        height: 40
                        radius: 8
                        color: "#e94560"

                        Text {
                            anchors.centerIn: parent
                            text: "Выход"
                            color: "white"
                            font.pixelSize: 13
                            font.bold: true
                            font.family: "Segoe UI"
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onEntered: parent.opacity = 0.9
                            onExited: parent.opacity = 1.0
                            onClicked: Database.logout()
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                radius: 12
                color: "#ffffff"
                border.color: "#0f3460"
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4

                    Repeater {
                        model: ["Мой профиль", "Активные услуги", "История"]

                        delegate: Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 8
                            color: currentTab === index ? "#1a1a2e" : "#f0f0f0"

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: 14
                                font.bold: true
                                font.family: "Segoe UI"
                                color: currentTab === index ? "white" : "#1a1a2e"
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onEntered: parent.opacity = 0.9
                                onExited: parent.opacity = 1.0
                                onClicked: {
                                    currentTab = index
                                    refreshData()
                                }
                            }
                        }
                    }
                }
            }

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: currentTab

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 12
                    color: "#ffffff"
                    border.color: "#0f3460"
                    border.width: 1

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: 24

                        ColumnLayout {
                            width: parent.width
                            spacing: 20

                            Text {
                                text: barberInfo.name || ""
                                font.pixelSize: 32
                                font.bold: true
                                color: "#1a1a2e"
                                font.family: "Segoe UI"
                            }

                            Rectangle { Layout.fillWidth: true; height: 1; color: "#0f3460" }

                            GridLayout {
                                columns: 2
                                rowSpacing: 16
                                columnSpacing: 24

                                Text { text: "Стаж:"; font.pixelSize: 16; color: "#0f3460"; font.bold: true; font.family: "Segoe UI" }
                                Text { text: (barberInfo.experience || "0") + " лет"; font.pixelSize: 16; color: "#1a1a2e"; font.family: "Segoe UI" }

                                Text { text: "Специализации:"; font.pixelSize: 16; color: "#0f3460"; font.bold: true; font.family: "Segoe UI" }
                                Text { text: barberInfo.specializations || "-"; font.pixelSize: 16; color: "#1a1a2e"; font.family: "Segoe UI"; wrapMode: Text.WordWrap }

                                Text { text: "Выполнено услуг:"; font.pixelSize: 16; color: "#0f3460"; font.bold: true; font.family: "Segoe UI" }
                                Text { text: (barberInfo.services_completed || "0").toString(); font.pixelSize: 16; color: "#1a1a2e"; font.family: "Segoe UI" }

                                Text { text: "Ставка:"; font.pixelSize: 16; color: "#0f3460"; font.bold: true; font.family: "Segoe UI" }
                                Text { text: (barberInfo.commission_rate || "0") + "%"; font.pixelSize: 16; color: "#1a1a2e"; font.family: "Segoe UI" }

                                Text { text: "Заработано:"; font.pixelSize: 16; color: "#0f3460"; font.bold: true; font.family: "Segoe UI" }
                                Text {
                                    text: {
                                        let earned = barberInfo.total_earned !== undefined ? barberInfo.total_earned : 0;
                                        return parseFloat(earned).toFixed(2) + " р";
                                    }
                                    font.pixelSize: 20; font.bold: true; color: "#0f3460"; font.family: "Segoe UI"
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 12
                    color: "#ffffff"
                    border.color: "#0f3460"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "Доступные запросы"
                            font.pixelSize: 20
                            font.bold: true
                            color: "#1a1a2e"
                            font.family: "Segoe UI"
                        }

                        ListView {
                            id: activeTasksList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 12
                            clip: true
                            model: []

                            delegate: Rectangle {
                                width: parent ? parent.width : 0
                                height: 110
                                radius: 12
                                color: "#f8f8f8"
                                border.color: "#0f3460"
                                border.width: 1

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 16
                                    spacing: 16

                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 4

                                        Text {
                                            text: "Клиент: " + (modelData.client_name || "-")
                                            font.pixelSize: 18
                                            font.bold: true
                                            color: "#1a1a2e"
                                            font.family: "Segoe UI"
                                        }
                                        Text {
                                            text: "Услуга: " + (modelData.haircut || "-")
                                            font.pixelSize: 14
                                            color: "#0f3460"
                                            font.family: "Segoe UI"
                                        }
                                        Text {
                                            text: "Время: " + (modelData.scheduled_time || "-")
                                            font.pixelSize: 14
                                            color: "#0f3460"
                                            font.family: "Segoe UI"
                                        }
                                        Text {
                                            text: "Сумма: " + (modelData.total_price || "0") + " р"
                                            font.pixelSize: 16
                                            font.bold: true
                                            color: "#0f3460"
                                            font.family: "Segoe UI"
                                        }
                                    }

                                    Rectangle {
                                        Layout.preferredWidth: 130
                                        Layout.preferredHeight: 44
                                        radius: 8
                                        color: modelData.status === statusPending ? "#1a1a2e" : (modelData.status === statusActive ? "#0f3460" : "#aaa")
                                        visible: modelData.status === statusPending || modelData.status === statusActive

                                        Text {
                                            anchors.centerIn: parent
                                            text: modelData.status === statusPending ? "Принять" : "Завершить"
                                            color: "white"
                                            font.pixelSize: 13
                                            font.bold: true
                                            font.family: "Segoe UI"
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            onEntered: parent.opacity = 0.9
                                            onExited: parent.opacity = 1.0
                                            onClicked: {
                                                if (modelData.status === statusPending) {
                                                    showStatus(Database.acceptTask(modelData.id))
                                                    refreshData()
                                                } else if (modelData.status === statusActive) {
                                                    showStatus(Database.completeTask(modelData.id))
                                                    refreshData()
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Нет активных запросов"
                                font.pixelSize: 18
                                color: "#0f3460"
                                font.family: "Segoe UI"
                                visible: activeTasksList.count === 0
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 12
                    color: "#ffffff"
                    border.color: "#0f3460"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            text: "История выполненных услуг"
                            font.pixelSize: 20
                            font.bold: true
                            color: "#1a1a2e"
                            font.family: "Segoe UI"
                        }

                        ListView {
                            id: historyList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 8
                            clip: true
                            model: []

                            delegate: Rectangle {
                                width: parent ? parent.width : 0
                                height: 56
                                radius: 8
                                color: index % 2 === 0 ? "#f8f8f8" : "#ffffff"
                                border.color: "#0f3460"
                                border.width: 1

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 12

                                    Text {
                                        text: modelData.client_name || "-"
                                        font.pixelSize: 16
                                        font.family: "Segoe UI"
                                        color: "#1a1a2e"
                                        Layout.fillWidth: true
                                    }
                                    Text {
                                        text: (modelData.total_price || "0") + " р"
                                        font.pixelSize: 16
                                        font.bold: true
                                        font.family: "Segoe UI"
                                        color: "#0f3460"
                                    }
                                    Text {
                                        text: modelData.completed_at || ""
                                        font.pixelSize: 13
                                        font.family: "Segoe UI"
                                        color: "#999"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: notifyRect
        width: 360
        height: 56
        radius: 8
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 24
        opacity: 0

        Text {
            id: notifyText
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 14
            font.bold: true
            font.family: "Segoe UI"
        }

        SequentialAnimation {
            id: notifyAnim
            NumberAnimation { target: notifyRect; property: "opacity"; from: 0; to: 1; duration: 200 }
            PauseAnimation { duration: 2200 }
            NumberAnimation { target: notifyRect; property: "opacity"; from: 1; to: 0; duration: 350 }
        }
    }
}
