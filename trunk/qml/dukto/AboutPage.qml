/* DUKTO - A simple, fast and multi-platform file transfer tool for LAN users
 * Copyright (C) 2011 Emanuele Colombo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

import QtQuick 1.0

Item {

    Rectangle {
        x: 27
        y: 15
        width: 64
        height: 64
        color: theme.color2
        Image {
            source: "TileGradient.png"
            anchors.fill: parent
        }
        Image {
            source: "DuktoMetroIcon.png"
            anchors.fill: parent
        }
    }

    SmoothText {
        y: 80
        x: 22
        font.pixelSize: 100
        text: "Dukto R5"
        color: theme.color4
    }
    SmoothText {
        x: 25
        y: 70 + 75
        font.pixelSize: 38
        text: "Created by Emanuele Colombo"
        color: theme.color5
    }
    SmoothText {
        x: 25
        y: 100 + 75
        font.pixelSize: 32
        text: "Website: <a href=\"http://www.msec.it/dukto/\">http://www.msec.it/dukto/</a>"
        onLinkActivated: Qt.openUrlExternally(link)
        color: theme.color5
    }

    SText {
        anchors.right: parent.right
        anchors.rightMargin: 40
        anchors.left: parent.left
        anchors.leftMargin: 25
        y: 240
        font.pixelSize: 12
        color: theme.color5
        wrapMode: "WordWrap"
        text: "This application and it's source code are released freely as open source project.<br>If you like this application please consider <a href=\"http://www.msec.it/\">making a donation</a>."
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
