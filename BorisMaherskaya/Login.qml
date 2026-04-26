import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: loginWindow
    width: Screen.width
    height: Screen.height
    visible: true
    title: "Система управления"

    property var adminWindowInstance: null
    property var guestWindowInstance: null

    function openBarber() {
        var list = Database.getBarberList()
        if (list.length === 0) {
            loginError.visible = true
            loginError.text = "Нет зарегистрированных сотрудников"
            return
        }
        barberDialog.barberList = list
        barberDialog.open()
    }

    function openAdmin() {
        if (Database.adminLogin(passwordField.text)) {
            loginError.visible = false
            loginDialog.close()
            var component = Qt.createComponent("Main.qml")
            if (component.status === Component.Ready) {
                adminWindowInstance = component.createObject(loginWindow, { "loginWindowRef": loginWindow })
                adminWindowInstance.showMaximized()
                loginWindow.hide()
            }
        } else {
            loginError.visible = true
            loginError.text = "Неверный пароль"
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1a1a2e" }
            GradientStop { position: 0.5; color: "#16213e" }
            GradientStop { position: 1.0; color: "#0f3460" }
        }

        Rectangle {
            anchors.centerIn: parent
            width: 450
            height: 500
            radius: 16
            color: "#ffffff"
            border.color: "#e94560"
            border.width: 2

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 32
                spacing: 24

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Система управления"
                    font.pixelSize: 32
                    font.bold: true
                    color: "#1a1a2e"
                    font.family: "Segoe UI"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Профессиональный сервис"
                    font.pixelSize: 16
                    color: "#0f3460"
                    font.family: "Segoe UI"
                }

                Item { Layout.preferredHeight: 20 }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    radius: 12
                    color: "#ffffff"
                    border.color: "#0f3460"
                    border.width: 2

                    Text {
                        anchors.centerIn: parent
                        text: "Вход для сотрудников"
                        color: "#0f3460"
                        font.pixelSize: 18
                        font.bold: true
                        font.family: "Segoe UI"
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onEntered: parent.opacity = 0.9
                        onExited: parent.opacity = 1.0
                        onClicked: openBarber()
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 60
                    radius: 12
                    color: "#1a1a2e"

                    Text {
                        anchors.centerIn: parent
                        text: "Вход для администратора"
                        color: "white"
                        font.pixelSize: 18
                        font.bold: true
                        font.family: "Segoe UI"
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onEntered: parent.opacity = 0.9
                        onExited: parent.opacity = 1.0
                        onClicked: loginDialog.open()
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
    }

    Dialog {
        id: loginDialog
        modal: true
        width: 400
        height: 340
        anchors.centerIn: parent
        title: "Авторизация"

        background: Rectangle {
            radius: 12
            color: "#ffffff"
            border.color: "#e94560"
            border.width: 2
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "Введите пароль"
                font.pixelSize: 20
                font.bold: true
                color: "#1a1a2e"
                font.family: "Segoe UI"
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                radius: 8
                color: "#f5f5f5"
                border.color: "#0f3460"

                TextInput {
                    id: passwordField
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    font.pixelSize: 16
                    font.family: "Segoe UI"
                    echoMode: TextInput.Password
                    verticalAlignment: TextInput.AlignVCenter
                    clip: true
                    color: "#1a1a2e"
                    onAccepted: openAdmin()

                    Text {
                        anchors.fill: parent
                        anchors.leftMargin: 0
                        text: "Пароль"
                        color: "#999"
                        font.pixelSize: 16
                        font.family: "Segoe UI"
                        verticalAlignment: Text.AlignVCenter
                        visible: !passwordField.text && !passwordField.activeFocus
                    }
                }
            }

            Text {
                id: loginError
                Layout.fillWidth: true
                visible: false
                text: "Неверный пароль"
                color: "#e94560"
                font.pixelSize: 13
                font.family: "Segoe UI"
                horizontalAlignment: Text.AlignHCenter
            }

            Item { Layout.preferredHeight: 10 }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    radius: 8
                    color: "#e0e0e0"
                    border.color: "#999"

                    Text {
                        anchors.centerIn: parent
                        text: "Отмена"
                        color: "#333"
                        font.pixelSize: 16
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
                            loginError.visible = false
                            passwordField.text = ""
                            loginDialog.close()
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    radius: 8
                    color: "#1a1a2e"

                    Text {
                        anchors.centerIn: parent
                        text: "Войти"
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                        font.family: "Segoe UI"
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onEntered: parent.opacity = 0.9
                        onExited: parent.opacity = 1.0
                        onClicked: openAdmin()
                    }
                }
            }
        }
    }

    Dialog {
        id: barberDialog
        modal: true
        width: 400
        height: 380
        anchors.centerIn: parent
        title: "Выберите сотрудника"
        property var barberList: []

        background: Rectangle {
            radius: 12
            color: "#ffffff"
            border.color: "#0f3460"
            border.width: 2
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 14

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "Выберите профиль"
                font.pixelSize: 20
                font.bold: true
                color: "#1a1a2e"
                font.family: "Segoe UI"
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: barberDialog.barberList
                clip: true

                delegate: Rectangle {
                    width: parent ? parent.width : 0
                    height: 48
                    radius: 8
                    color: index % 2 === 0 ? "#f0f0f0" : "#ffffff"

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 16
                        font.family: "Segoe UI"
                        color: "#1a1a2e"
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onEntered: parent.opacity = 0.8
                        onExited: parent.opacity = 1.0
                        onClicked: {
                            if (Database.loginAsBarber(modelData)) {
                                barberDialog.close()
                                var component = Qt.createComponent("Guest.qml")
                                if (component.status === Component.Ready) {
                                    guestWindowInstance = component.createObject(loginWindow, { "loginWindowRef": loginWindow })
                                    guestWindowInstance.showMaximized()
                                    loginWindow.hide()
                                }
                            }
                        }
                    }
                }
            }

            Button {
                text: "Отмена"
                Layout.alignment: Qt.AlignRight
                onClicked: barberDialog.close()
            }
        }
    }
}
