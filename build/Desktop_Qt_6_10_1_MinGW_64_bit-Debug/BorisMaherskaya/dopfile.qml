import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialog
    modal: true
    anchors.centerIn: parent
    width: 600
    height: 650
    title: editMode ? "Редактирование" : "Добавление"

    onClosed: destroy()

    property bool editMode: false
    property var recordId: null
    property string currentTable: Database.getCurrentTable()
    property var fieldsModel: []
    property var loadedRecord: ({})
    property var allServices: []
    property var selectedSpecs: []
    property bool hasSpecializations: false

    function showStatus(message) {
        let isError = message.toLowerCase().indexOf("ошибка") !== -1
        notifyRect.color = isError ? "#e94560" : "#0f3460"
        notifyText.text = message
        notifyAnim.restart()
    }

    function addSpec() {
        if (!specCombo || specCombo.currentIndex < 0 || specCombo.currentIndex >= allServices.length) return

        let item = allServices[specCombo.currentIndex]
        let alreadyExists = false
        for (let i = 0; i < selectedSpecs.length; i++) {
            if (selectedSpecs[i].id === item.id) {
                alreadyExists = true
                break
            }
        }
        if (!alreadyExists) {
            let newList = []
            for (let i = 0; i < selectedSpecs.length; i++) {
                newList.push(selectedSpecs[i])
            }
            newList.push(item)
            selectedSpecs = newList
        }
    }

    function removeSpec(index) {
        let newList = []
        for (let i = 0; i < selectedSpecs.length; i++) {
            if (i !== index) newList.push(selectedSpecs[i])
        }
        selectedSpecs = newList
    }

    function loadValues() {
        fieldsModel = Database.getColumnsMetadata()
        allServices = Database.getAllServices()
        selectedSpecs = []
        hasSpecializations = false

        for (let i = 0; i < fieldsModel.length; i++) {
            if (fieldsModel[i].isSpecialization) {
                hasSpecializations = true
                break
            }
        }

        if (editMode && recordId !== null) {
            loadedRecord = Database.getRecordData(recordId)
            if (loadedRecord.specializations && hasSpecializations) {
                let specNames = loadedRecord.specializations.toString().split(", ")
                for (let i = 0; i < specNames.length; i++) {
                    let name = specNames[i].trim()
                    for (let j = 0; j < allServices.length; j++) {
                        if (allServices[j].name === name) {
                            selectedSpecs.push(allServices[j])
                            break
                        }
                    }
                }
            }
        }

        Qt.callLater(function() {
            for (let i = 0; i < fieldRepeater.count; ++i) {
                let item = fieldRepeater.itemAt(i)
                if (item && item.fieldInput && !item.isHidden) {
                    if (editMode && loadedRecord[item.fieldName] !== undefined) {
                        item.fieldInput.text = loadedRecord[item.fieldName].toString()
                    }
                }
            }
        })
    }

    function saveData() {
        let values = []
        let requiredError = false
        let specFieldIndex = -1
        let valueIndex = 0

        for (let i = 0; i < fieldsModel.length; i++) {
            let field = fieldsModel[i]
            if (field.isHidden) continue

            if (field.isSpecialization) {
                specFieldIndex = valueIndex
                values.push("")
                valueIndex++
                continue
            }

            let item = fieldRepeater.itemAt(i)
            let val = ""
            if (item && item.fieldInput) {
                val = item.fieldInput.text.trim()
            }

            if (field.required && (!val || val === "")) {
                requiredError = true
            }
            values.push(val)
            valueIndex++
        }

        if (specFieldIndex >= 0 && specFieldIndex < values.length) {
            let specString = ""
            for (let i = 0; i < selectedSpecs.length; i++) {
                if (i > 0) specString += ", "
                specString += selectedSpecs[i].name
            }
            values[specFieldIndex] = specString

            if (specString === "" && hasSpecializations) {
                for (let i = 0; i < fieldsModel.length; i++) {
                    if (fieldsModel[i].isSpecialization && fieldsModel[i].required) {
                        requiredError = true
                        break
                    }
                }
            }
        }

        if (requiredError) {
            showStatus("Ошибка: заполните обязательные поля")
            return
        }

        let result = editMode
                   ? Database.updateRecord(recordId, values)
                   : Database.insertRecord(values)

        showStatus(result)
        if (result.toLowerCase().indexOf("ошибка") === -1)
            dialog.close()
    }

    Component.onCompleted: loadValues()

    background: Rectangle {
        radius: 12
        color: "#ffffff"
        border.color: "#0f3460"
        border.width: 1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: editMode ? ("Редактирование: " + Database.currentTableDisplayName) : ("Новая запись: " + Database.currentTableDisplayName)
            font.pixelSize: 22
            font.bold: true
            color: "#1a1a2e"
            font.family: "Segoe UI"
        }

        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: availableWidth

            ColumnLayout {
                width: scrollView.availableWidth
                spacing: 14

                Repeater {
                    id: fieldRepeater
                    model: fieldsModel

                    delegate: ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        visible: !modelData.isHidden

                        property string fieldName: modelData.name
                        property bool isHidden: modelData.isHidden
                        property alias fieldInput: textField

                        Text {
                            text: modelData.display + (modelData.required ? " *" : "")
                            font.pixelSize: 14
                            font.bold: true
                            color: "#1a1a2e"
                            font.family: "Segoe UI"
                            visible: !modelData.isSpecialization
                        }

                        TextField {
                            id: textField
                            Layout.fillWidth: true
                            placeholderText: "Введите значение"
                            font.family: "Segoe UI"
                            visible: !modelData.isSpecialization
                        }
                    }
                }

                // Блок специализаций
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: hasSpecializations

                    Text {
                        text: "Специализации (выберите из услуг):"
                        font.pixelSize: 14
                        font.bold: true
                        color: "#1a1a2e"
                        font.family: "Segoe UI"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        ComboBox {
                            id: specCombo
                            Layout.fillWidth: true
                            textRole: "displayName"
                            model: allServices
                        }

                        Rectangle {
                            Layout.preferredWidth: 110
                            Layout.preferredHeight: 40
                            radius: 8
                            color: "#1a1a2e"

                            Text {
                                anchors.centerIn: parent
                                text: "+ Добавить"
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
                                onClicked: dialog.addSpec()
                            }
                        }
                    }

                    ColumnLayout {
                        visible: selectedSpecs.length > 0
                        spacing: 4

                        Text {
                            text: "Выбрано:"
                            font.pixelSize: 13
                            font.bold: true
                            color: "#0f3460"
                            font.family: "Segoe UI"
                        }

                        Repeater {
                            model: selectedSpecs
                            delegate: Rectangle {
                                Layout.fillWidth: true
                                height: 36
                                radius: 8
                                color: "#f0f0f0"
                                border.color: "#0f3460"

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 8

                                    Text {
                                        text: modelData.displayName
                                        font.pixelSize: 13
                                        font.family: "Segoe UI"
                                        color: "#1a1a2e"
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }

                                    Rectangle {
                                        Layout.preferredWidth: 24
                                        Layout.preferredHeight: 24
                                        radius: 12
                                        color: "#e94560"

                                        Text {
                                            anchors.centerIn: parent
                                            text: "x"
                                            color: "white"
                                            font.pixelSize: 16
                                            font.bold: true
                                            font.family: "Segoe UI"
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: dialog.removeSpec(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Button {
                Layout.fillWidth: true
                text: "Отмена"
                onClicked: dialog.close()
            }

            Button {
                Layout.fillWidth: true
                text: "Сохранить"
                onClicked: saveData()
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
            PauseAnimation { duration: 2000 }
            NumberAnimation { target: notifyRect; property: "opacity"; from: 1; to: 0; duration: 350 }
        }
    }
}
