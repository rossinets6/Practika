import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Window {
    id: mainWindow
    width: Screen.width
    height: Screen.height
    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint | Qt.Window
    title: "Панель администратора"

    property string selectedTable: "Haircuts"
    property var contextRecordId: null
    property var loginWindowRef: null
    property string taskStatusText: ""

    Timer {
        id: refreshTimer
        interval: 150
        repeat: false
        onTriggered: {
            if (!mainWindow) return
            if (typeof Database === "undefined" || !Database) return
            try {
                Database.loadTable(mainWindow.selectedTable, searchField.text.trim())
                headerModel.updateHeaders()
            } catch(e) {}
        }
    }

    function refreshTable() {
        if (!mainWindow) return
        refreshTimer.start()
    }

    function showStatus(message) {
        if (!mainWindow) return
        try {
            let isError = message.toLowerCase().indexOf("ошибка") !== -1 || message.toLowerCase().indexOf("невозможно") !== -1
            notifyRect.color = isError ? "#e94560" : "#0f3460"
            notifyText.text = message
            notifyAnim.restart()
        } catch(e) {}
    }

    function openAddDialog() {
        if (!mainWindow && typeof Database === "undefined") return
        try {
            let st = mainWindow.selectedTable
            if (st === "Tasks") {
                taskDialog.reloadData()
                taskDialog.open()
                return
            }
            if (st === "CashJournal") {
                showStatus("Журнал только для просмотра")
                return
            }
            var component = Qt.createComponent("dopfile.qml")
            if (component.status === Component.Ready) {
                var dialog = component.createObject(mainWindow, { editMode: false, recordId: null })
                if (dialog) dialog.open()
            }
        } catch(e) {}
    }

    function openEditDialog(recordId) {
        if (!mainWindow) return
        try {
            let st = mainWindow.selectedTable
            if (st === "Tasks") {
                mainWindow.contextRecordId = recordId
                taskContextMenu.popup()
                return
            }
            if (st === "CashJournal") return

            var component = Qt.createComponent("dopfile.qml")
            if (component.status === Component.Ready) {
                var dialog = component.createObject(mainWindow, { editMode: true, recordId: recordId })
                if (dialog) dialog.open()
            }
        } catch(e) {}
    }

    Connections {
        target: Database
        function onTableChanged() {
            if (!mainWindow) return
            try { headerModel.updateHeaders() } catch(e) {}
        }
        function onLogoutSuccess() {
            if (!mainWindow || !mainWindow.visible) return
            try {
                if (loginWindowRef) {
                    loginWindowRef.show()
                    mainWindow.destroy()
                }
            } catch(e) {}
        }
    }

    Component.onCompleted: {
        if (mainWindow) refreshTable()
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#f5f5f5" }
            GradientStop { position: 0.5; color: "#e8e8e8" }
            GradientStop { position: 1.0; color: "#d0d0d0" }
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 16

            Rectangle {
                Layout.preferredWidth: 240
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
                        Layout.alignment: Qt.AlignHCenter
                        text: "Меню"
                        font.pixelSize: 24
                        font.bold: true
                        color: "#1a1a2e"
                        font.family: "Segoe UI"
                    }

                    Rectangle { Layout.fillWidth: true; height: 1; color: "#e0e0e0" }

                    Repeater {
                        model: [
                            { key: "Haircuts", title: "Прически" },
                            { key: "Services", title: "Услуги" },
                            { key: "Barbers", title: "Сотрудники" },
                            { key: "Products", title: "Товары" },
                            { key: "Clients", title: "Клиенты" },
                            { key: "Tasks", title: "Запросы" },
                            { key: "CashJournal", title: "Касса" }
                        ]

                        delegate: Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 44
                            radius: 8
                            color: selectedTable === modelData.key ? "#1a1a2e" : "#f0f0f0"
                            border.color: selectedTable === modelData.key ? "#e94560" : "#e0e0e0"

                            Text {
                                anchors.centerIn: parent
                                text: modelData.title
                                font.pixelSize: 14
                                font.bold: true
                                font.family: "Segoe UI"
                                color: selectedTable === modelData.key ? "white" : "#1a1a2e"
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    if (!mainWindow) return
                                    if (mainWindow.selectedTable !== modelData.key) {
                                        mainWindow.selectedTable = modelData.key
                                        mainWindow.refreshTable()
                                    }
                                }
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        radius: 8
                        color: "#e94560"
                        Text {
                            anchors.centerIn: parent
                            text: "Выход"
                            color: "white"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Segoe UI"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (!mainWindow) return
                                try { if (Database) Database.logout() } catch(e) {}
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 12

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
                            text: (typeof Database !== "undefined" && Database) ? Database.currentTableDisplayName : ""
                            font.pixelSize: 24
                            font.bold: true
                            color: "#1a1a2e"
                            font.family: "Segoe UI"
                        }
                        Item { Layout.fillWidth: true }
                        Rectangle {
                            Layout.preferredWidth: 280
                            Layout.preferredHeight: 40
                            radius: 8
                            color: "#f5f5f5"
                            border.color: "#0f3460"
                            TextInput {
                                id: searchField
                                anchors.fill: parent
                                anchors.leftMargin: 16
                                anchors.rightMargin: 16
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                verticalAlignment: TextInput.AlignVCenter
                                clip: true
                                color: "#1a1a2e"
                                onTextChanged: { if (mainWindow) mainWindow.refreshTable() }
                                Text {
                                    anchors.fill: parent
                                    anchors.leftMargin: 2
                                    text: "Поиск..."
                                    color: "#999"
                                    font.pixelSize: 14
                                    font.family: "Segoe UI"
                                    verticalAlignment: Text.AlignVCenter
                                    visible: !searchField.text && !searchField.activeFocus
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 12
                    color: "white"
                    border.color: "#0f3460"
                    border.width: 1
                    clip: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 44
                            color: "#1a1a2e"
                            clip: true

                            Row {
                                anchors.fill: parent
                                anchors.margins: 2
                                Repeater {
                                    model: headerModel
                                    delegate: Rectangle {
                                        width: parent ? parent.width / Math.max(headerModel.count, 1) : 100
                                        height: parent ? parent.height : 44
                                        color: "transparent"
                                        Text {
                                            anchors.fill: parent
                                            anchors.margins: 8
                                            text: model.headerText || ""
                                            color: "white"
                                            font.pixelSize: 13
                                            font.bold: true
                                            font.family: "Segoe UI"
                                            elide: Text.ElideRight
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }
                                }
                            }
                        }

                        TableView {
                            id: tableView
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            model: Database.tableModel
                            clip: true
                            rowSpacing: 0
                            columnSpacing: 0

                            columnWidthProvider: function(column) {
                                try {
                                    let w = tableView.width; if (w <= 0) w = 800
                                    let cnt = tableView.model ? tableView.model.columnCount() : 1
                                    if (cnt <= 0) cnt = 1
                                    return w / cnt
                                } catch(e) { return 100 }
                            }

                            delegate: Rectangle {
                                required property int row
                                required property int column
                                implicitHeight: 42
                                color: row % 2 === 0 ? "#f8f8f8" : "white"

                                Text {
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    text: {
                                        try {
                                            let m = tableView.model; if (!m) return ""
                                            let idx = m.index(row, column)
                                            return idx ? (idx.data(Qt.DisplayRole) || "") : ""
                                        } catch(e) { return "" }
                                    }
                                    font.pixelSize: 14
                                    font.family: "Segoe UI"
                                    color: "#1a1a2e"
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    elide: Text.ElideRight
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    onClicked: function(mouse) {
                                        try {
                                            if (!mainWindow) return
                                            let m = tableView.model; if (!m) return
                                            let rid = m.index(row, 0).data(Qt.DisplayRole)
                                            if (!rid) return
                                            if (mouse.button === Qt.RightButton) {
                                                mainWindow.contextRecordId = rid
                                                if (mainWindow.selectedTable === "Tasks") {
                                                    let statusIdx = 5
                                                    mainWindow.taskStatusText = m.index(row, statusIdx).data()
                                                    taskContextMenu.popup()
                                                } else {
                                                    actionMenu.popup()
                                                }
                                            } else {
                                                mainWindow.openEditDialog(rid)
                                            }
                                        } catch(e) {}
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 56
                    visible: selectedTable === "CashJournal"
                    radius: 8
                    color: "#f8f8f8"
                    border.color: "#0f3460"
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 16

                        Text {
                            text: "Итого за сегодня:"
                            font.pixelSize: 16
                            font.bold: true
                            color: "#1a1a2e"
                            font.family: "Segoe UI"
                        }

                        Item { Layout.fillWidth: true }

                        Rectangle {
                            Layout.preferredWidth: 160
                            Layout.preferredHeight: 36
                            radius: 6
                            color: "#ffffff"
                            border.color: "#0f3460"

                            Text {
                                anchors.centerIn: parent
                                text: "Клиентов: " + (typeof Database !== "undefined" && Database ? Database.getTodayClientsCount() : 0)
                                font.pixelSize: 14
                                font.bold: true
                                color: "#1a1a2e"
                                font.family: "Segoe UI"
                            }
                        }

                        Rectangle {
                            Layout.preferredWidth: 180
                            Layout.preferredHeight: 36
                            radius: 6
                            color: "#1a1a2e"

                            Text {
                                anchors.centerIn: parent
                                text: "Сумма: " + (typeof Database !== "undefined" && Database ? Database.getCurrentDayTotal().toFixed(0) : 0) + " р"
                                font.pixelSize: 14
                                font.bold: true
                                color: "white"
                                font.family: "Segoe UI"
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    Rectangle {
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 40
                        radius: 8
                        color: "#ffffff"
                        border.color: "#0f3460"
                        Text {
                            anchors.centerIn: parent
                            text: "PDF"
                            color: "#1a1a2e"
                            font.pixelSize: 13
                            font.bold: true
                            font.family: "Segoe UI"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: { try { pdfDialog.open() } catch(e) {} }
                        }
                    }
                    Rectangle {
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 40
                        radius: 8
                        color: "#ffffff"
                        border.color: "#0f3460"
                        Text {
                            anchors.centerIn: parent
                            text: "HTML"
                            color: "#1a1a2e"
                            font.pixelSize: 13
                            font.bold: true
                            font.family: "Segoe UI"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: { try { htmlDialog.open() } catch(e) {} }
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Rectangle {
                        Layout.preferredWidth: 140
                        Layout.preferredHeight: 40
                        radius: 8
                        color: "#1a1a2e"
                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: 14
                            font.bold: true
                            font.family: "Segoe UI"
                            text: {
                                try {
                                    let st = mainWindow ? mainWindow.selectedTable : ""
                                    if (st === "Tasks") return "Создать"
                                    if (st === "CashJournal") return "Очистить"
                                    return "Добавить"
                                } catch(e) { return "Добавить" }
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                try {
                                    if (mainWindow.selectedTable === "CashJournal") {
                                        mainWindow.showStatus(Database.clearCashJournal())
                                    } else {
                                        mainWindow.openAddDialog()
                                    }
                                } catch(e) {}
                            }
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: headerModel
        function updateHeaders() {
            if (!headerModel) return
            try {
                clear()
                let m = (typeof Database !== "undefined" && Database) ? Database.tableModel : null
                if (!m) return
                let cnt = m.columnCount()
                for (let i = 0; i < cnt; i++) {
                    append({headerText: m.headerData(i, Qt.Horizontal).toString()})
                }
            } catch(e) {}
        }
    }

    Menu {
        id: taskContextMenu
        MenuItem {
            text: "В работу"
            visible: taskStatusText === "Ожидает"
            onTriggered: {
                if (!mainWindow || mainWindow.contextRecordId === null) return
                try {
                    mainWindow.showStatus(Database.markTaskStarted(mainWindow.contextRecordId))
                    mainWindow.refreshTable()
                } catch(e) {}
            }
        }
        MenuItem {
            text: "Выполнено"
            visible: taskStatusText === "В работе" || taskStatusText === "Ожидает"
            onTriggered: {
                if (!mainWindow || mainWindow.contextRecordId === null) return
                try {
                    mainWindow.showStatus(Database.markTaskCompleted(mainWindow.contextRecordId))
                    mainWindow.refreshTable()
                } catch(e) {}
            }
        }
        MenuItem {
            text: "Отказ"
            visible: taskStatusText === "Ожидает"
            onTriggered: {
                if (!mainWindow || mainWindow.contextRecordId === null) return
                try {
                    mainWindow.showStatus(Database.markTaskCancelled(mainWindow.contextRecordId))
                    mainWindow.refreshTable()
                } catch(e) {}
            }
        }
        MenuItem {
            text: "Удалить"
            visible: taskStatusText === "Выполнен" || taskStatusText === "Отказ"
            onTriggered: {
                if (!mainWindow || mainWindow.contextRecordId === null) return
                try {
                    mainWindow.showStatus(Database.deleteRecord(mainWindow.contextRecordId))
                    mainWindow.refreshTable()
                } catch(e) {}
            }
        }
    }

    Menu {
        id: actionMenu
        MenuItem {
            text: "Редактировать"
            onTriggered: {
                try { if (mainWindow && mainWindow.contextRecordId !== null) mainWindow.openEditDialog(mainWindow.contextRecordId) } catch(e) {}
            }
        }
        MenuItem {
            text: "Удалить"
            onTriggered: {
                try {
                    if (mainWindow && mainWindow.contextRecordId !== null) {
                        mainWindow.showStatus(Database.deleteRecord(mainWindow.contextRecordId))
                        mainWindow.refreshTable()
                    }
                } catch(e) {}
            }
        }
    }

    Dialog {
        id: taskDialog
        modal: true
        width: 640
        height: 700
        anchors.centerIn: parent
        title: "Создать запрос"

        property var haircutsModel: []
        property var servicesModel: []
        property var productsModel: []
        property var barbersModel: []
        property var clientsModel: []
        property var selectedServices: []
        property var selectedProducts: []
        property var selectedBarbers: []
        property var selectedClientId: 0

        function reloadData() {
            try {
                if (!mainWindow) return
                taskDialog.haircutsModel = Database.getCatalogData("haircuts")
                taskDialog.servicesModel = Database.getCatalogData("services")
                taskDialog.productsModel = Database.getCatalogData("products")
                taskDialog.barbersModel = Database.getBarbersList()
                taskDialog.clientsModel = Database.getClientsList()
                taskDialog.selectedServices = []
                taskDialog.selectedProducts = []
                taskDialog.selectedBarbers = []
                taskTime.text = ""
            } catch(e) {}
        }

        Component.onCompleted: reloadData()

        background: Rectangle { radius: 12; color: "#ffffff"; border.color: "#0f3460"; border.width: 1 }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 12

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                ColumnLayout {
                    width: parent.width
                    spacing: 12

                    Text { text: "Клиент *"; font.pixelSize: 14; color: "#1a1a2e"; font.bold: true; font.family: "Segoe UI" }
                    ComboBox {
                        id: taskClient
                        Layout.fillWidth: true
                        textRole: "name"
                        valueRole: "id"
                        model: taskDialog.clientsModel
                        onCurrentIndexChanged: {
                            if (currentIndex >= 0 && currentIndex < taskDialog.clientsModel.length) {
                                taskDialog.selectedClientId = taskDialog.clientsModel[currentIndex].id
                            } else {
                                taskDialog.selectedClientId = 0
                            }
                        }
                    }

                    Text { text: "Прическа:"; font.pixelSize: 14; color: "#1a1a2e"; font.bold: true; font.family: "Segoe UI" }
                    ComboBox { id: taskHaircut; Layout.fillWidth: true; textRole: "name"; valueRole: "id"; model: taskDialog.haircutsModel }

                    Text { text: "Услуги:"; font.pixelSize: 14; color: "#1a1a2e"; font.bold: true; font.family: "Segoe UI" }
                    RowLayout { Layout.fillWidth: true; spacing: 8
                        ComboBox { id: serviceCombo; Layout.fillWidth: true; textRole: "name"; valueRole: "id"; model: taskDialog.servicesModel }
                        Button {
                            text: "+"
                            onClicked: {
                                try {
                                    let idx = serviceCombo.currentIndex
                                    let list = taskDialog.servicesModel
                                    if (idx < 0 || idx >= list.length) return
                                    let item = list[idx]
                                    let arr = taskDialog.selectedServices.slice()
                                    let exists = false
                                    for (let i = 0; i < arr.length; i++) { if (arr[i].id == item.id) { exists = true; break } }
                                    if (!exists) { arr.push({id: item.id, name: item.name}); taskDialog.selectedServices = arr }
                                } catch(e) {}
                            }
                        }
                    }
                    ColumnLayout { visible: taskDialog.selectedServices.length > 0; spacing: 4
                        Repeater {
                            model: taskDialog.selectedServices
                            delegate: Rectangle {
                                Layout.fillWidth: true; height: 36; radius: 8; color: "#f0f0f0"; border.color: "#0f3460"
                                RowLayout { anchors.fill: parent; anchors.margins: 8
                                    Text { text: modelData.name; font.pixelSize: 13; color: "#1a1a2e"; font.family: "Segoe UI"; Layout.fillWidth: true }
                                    Button { text: "x"; flat: true
                                        onClicked: {
                                            try {
                                                let arr = taskDialog.selectedServices.slice(); arr.splice(index, 1)
                                                taskDialog.selectedServices = arr
                                            } catch(e) {}
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Text { text: "Товары:"; font.pixelSize: 14; color: "#1a1a2e"; font.bold: true; font.family: "Segoe UI" }
                    RowLayout { Layout.fillWidth: true; spacing: 8
                        ComboBox { id: productCombo; Layout.fillWidth: true; textRole: "name"; valueRole: "id"; model: taskDialog.productsModel }
                        Button {
                            text: "+"
                            onClicked: {
                                try {
                                    let idx = productCombo.currentIndex
                                    let list = taskDialog.productsModel
                                    if (idx < 0 || idx >= list.length) return
                                    let item = list[idx]
                                    let arr = taskDialog.selectedProducts.slice()
                                    let exists = false
                                    for (let i = 0; i < arr.length; i++) { if (arr[i].id == item.id) { exists = true; break } }
                                    if (!exists) { arr.push({id: item.id, name: item.name}); taskDialog.selectedProducts = arr }
                                } catch(e) {}
                            }
                        }
                    }
                    ColumnLayout { visible: taskDialog.selectedProducts.length > 0; spacing: 4
                        Repeater {
                            model: taskDialog.selectedProducts
                            delegate: Rectangle {
                                Layout.fillWidth: true; height: 36; radius: 8; color: "#f0f0f0"; border.color: "#0f3460"
                                RowLayout { anchors.fill: parent; anchors.margins: 8
                                    Text { text: modelData.name; font.pixelSize: 13; color: "#1a1a2e"; font.family: "Segoe UI"; Layout.fillWidth: true }
                                    Button { text: "x"; flat: true
                                        onClicked: {
                                            try {
                                                let arr = taskDialog.selectedProducts.slice(); arr.splice(index, 1)
                                                taskDialog.selectedProducts = arr
                                            } catch(e) {}
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Text { text: "Сотрудники *"; font.pixelSize: 14; color: "#1a1a2e"; font.bold: true; font.family: "Segoe UI" }
                    RowLayout { Layout.fillWidth: true; spacing: 8
                        ComboBox { id: barberCombo; Layout.fillWidth: true; textRole: "name"; valueRole: "id"; model: taskDialog.barbersModel }
                        Button {
                            text: "+"
                            onClicked: {
                                try {
                                    let idx = barberCombo.currentIndex
                                    let list = taskDialog.barbersModel
                                    if (idx < 0 || idx >= list.length) return
                                    let item = list[idx]
                                    let arr = taskDialog.selectedBarbers.slice()
                                    let exists = false
                                    for (let i = 0; i < arr.length; i++) { if (arr[i].id == item.id) { exists = true; break } }
                                    if (!exists) { arr.push({id: item.id, name: item.name}); taskDialog.selectedBarbers = arr }
                                } catch(e) {}
                            }
                        }
                    }
                    ColumnLayout { visible: taskDialog.selectedBarbers.length > 0; spacing: 4
                        Repeater {
                            model: taskDialog.selectedBarbers
                            delegate: Rectangle {
                                Layout.fillWidth: true; height: 36; radius: 8; color: "#f0f0f0"; border.color: "#0f3460"
                                RowLayout { anchors.fill: parent; anchors.margins: 8
                                    Text { text: modelData.name; font.pixelSize: 13; color: "#1a1a2e"; font.family: "Segoe UI"; Layout.fillWidth: true }
                                    Button { text: "x"; flat: true
                                        onClicked: {
                                            try {
                                                let arr = taskDialog.selectedBarbers.slice(); arr.splice(index, 1)
                                                taskDialog.selectedBarbers = arr
                                            } catch(e) {}
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Text { text: "Время (ЧЧ:ММ) *"; font.pixelSize: 14; color: "#1a1a2e"; font.bold: true; font.family: "Segoe UI" }
                    TextField { id: taskTime; Layout.fillWidth: true; inputMask: "00:00"; placeholderText: "ЧЧ:ММ"; font.family: "Segoe UI" }
                }
            }

            RowLayout { Layout.fillWidth: true; spacing: 12
                Button { Layout.fillWidth: true; text: "Отмена"; onClicked: { try { taskDialog.close() } catch(e) {} } }
                Button {
                    Layout.fillWidth: true; text: "Создать"
                    onClicked: {
                        try {
                            if (!mainWindow) return
                            if (taskDialog.selectedClientId <= 0) { mainWindow.showStatus("Ошибка: выберите клиента"); return }
                            if (taskDialog.selectedBarbers.length === 0) { mainWindow.showStatus("Ошибка: выберите сотрудника"); return }
                            if (!taskTime.text.trim() || taskTime.text.length < 5) { mainWindow.showStatus("Ошибка: укажите время"); return }

                            let sids = []; for (let i = 0; i < taskDialog.selectedServices.length; i++) sids.push(taskDialog.selectedServices[i].id)
                            let pids = []; for (let i = 0; i < taskDialog.selectedProducts.length; i++) pids.push(taskDialog.selectedProducts[i].id)
                            let bids = []; for (let i = 0; i < taskDialog.selectedBarbers.length; i++) bids.push(taskDialog.selectedBarbers[i].id)

                            mainWindow.showStatus(Database.createTask({
                                clientName: taskClient.currentText,
                                clientId: taskDialog.selectedClientId,
                                haircutId: taskHaircut.currentValue || "",
                                serviceIds: sids.join(","),
                                productIds: pids.join(","),
                                barberIds: bids.join(","),
                                scheduledTime: taskTime.text.trim()
                            }))
                            taskDialog.close()
                            mainWindow.refreshTable()
                        } catch(e) {}
                    }
                }
            }
        }
    }

    FileDialog {
        id: pdfDialog; title: "Сохранить PDF"; fileMode: FileDialog.SaveFile; nameFilters: ["PDF (*.pdf)"]
        onAccepted: { if (mainWindow) try { mainWindow.showStatus(Database.exportToPDF(selectedFile)) } catch(e) {} }
    }
    FileDialog {
        id: htmlDialog; title: "Сохранить HTML"; fileMode: FileDialog.SaveFile; nameFilters: ["HTML (*.html)"]
        onAccepted: { if (mainWindow) try { mainWindow.showStatus(Database.exportToHTML(selectedFile)) } catch(e) {} }
    }

    Rectangle {
        id: notifyRect; width: 360; height: 56; radius: 8
        anchors.right: parent.right; anchors.bottom: parent.bottom; anchors.margins: 24; opacity: 0
        Text { id: notifyText; anchors.centerIn: parent; color: "white"; font.pixelSize: 14; font.bold: true; font.family: "Segoe UI" }
        SequentialAnimation {
            id: notifyAnim
            NumberAnimation { target: notifyRect; property: "opacity"; from: 0; to: 1; duration: 200 }
            PauseAnimation { duration: 2000 }
            NumberAnimation { target: notifyRect; property: "opacity"; from: 1; to: 0; duration: 350 }
        }
    }
}
