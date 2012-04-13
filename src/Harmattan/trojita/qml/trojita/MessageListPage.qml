import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page {
    property alias model: view.model

    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    Component {
        id: messageItemDelegate

        Item {
            function formatMailAddress(items) {
                if (items[0] !== null) {
                    return items[0] + " <" + items[2] + "@" + items[3] + ">"
                } else {
                    return items[2] + "@" + items[3]
                }
            }

            width: parent.width
            height: 120

            Column {
                visible: isFetched
                Label {
                    font: UiConstants.TitleFont
                    maximumLineCount: 1
                    elide: Text.ElideRight
                    width: view.width
                    text: subject === undefined ? "" : subject
                }
                Label {
                    font: UiConstants.SubtitleFont
                    maximumLineCount: 1
                    elide: Text.ElideRight
                    width: view.width
                    // FIXME: multiple/no addresses...
                    text: from === undefined ? "" : formatMailAddress(from[0])
                }
                Text {
                    font: UiConstants.BodyTextFont
                    // if there's a better way to compare QDateTime::date with "today", well, please do tell me
                    text: Qt.formatDate(date, "YYYY-mm-dd") == Qt.formatDate(new Date(), "YYYY-mm-dd") ?
                              Qt.formatTime(date) : Qt.formatDate(date)
                }
            }
            Label {
                text: qsTr("Message is loading...")
                visible: !isFetched
                anchors.centerIn: parent
                platformStyle: LabelStyle {
                    fontFamily: "Nokia Pure Text Light"
                    fontPixelSize: 40
                    textColor: "#a0a0a0"
                }
            }
        }
    }

    ListView {
        signal messageSelected(string mailbox)

        id: view
        anchors.fill: parent
        delegate: messageItemDelegate
    }
}