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

#include "buddylistitemmodel.h"

#include <QUrl>
#include <QRegExp>

#include "platform.h"
#include "peer.h"

BuddyListItemModel::BuddyListItemModel() :
    QStandardItemModel(NULL)
{
    QHash<int, QByteArray> roleNames;
    roleNames[Ip] = "ip";
    roleNames[Port] = "port";
    roleNames[Username] = "username";
    roleNames[System] = "system";
    roleNames[Platform] = "platform";
    roleNames[GenericAvatar] = "generic";
    roleNames[Avatar] = "avatar";
    roleNames[OsLogo] = "oslogo";
    roleNames[ShowBack] = "showback";
    setRoleNames(roleNames);
}

void BuddyListItemModel::addMeElement()
{
    addBuddy("",
             0,
             Platform::getSystemUsername() + " (You)",
             Platform::getHostname(),
             Platform::getPlatformName(),
             QUrl::fromLocalFile(Platform::getAvatarPath()));
}

void BuddyListItemModel::addIpElement()
{
    addBuddy("IP",
             0,
             "IP connection",
             "Send data to a remote device",
             "IP",
             QUrl(""));
}

void BuddyListItemModel::addBuddy(QString ip, qint16 port, QString username, QString system, QString platform, QUrl avatarPath)
{
    QStandardItem* it = NULL;
    bool add = true;

    // Check if the same IP is alreay in the buddy list
    if (mItemsMap.contains(ip)) {
        it = mItemsMap[ip];
        add = false;
    }
    else
        it = new QStandardItem();
    it->setData(ip, BuddyListItemModel::Ip);
    it->setData(port, BuddyListItemModel::Port);
    it->setData(false, BuddyListItemModel::ShowBack);

    // Set (or update) data
    it->setData(username, BuddyListItemModel::Username);
    if (ip != "IP")
        it->setData("at " + system, BuddyListItemModel::System);
    else
        it->setData(system, BuddyListItemModel::System);
    it->setData(platform, BuddyListItemModel::Platform);
    it->setData(avatarPath, BuddyListItemModel::Avatar);

    // Update generic avatar
    if ((platform.toLower() == "symbian") || (platform.toLower() == "android") || (platform.toLower() == "ios") || (platform.toLower() == "blackberry") || (platform.toLower() == "windowsphone"))
        it->setData("SmartphoneLogo.png", BuddyListItemModel::GenericAvatar);
    else if (platform.toLower() == "ip")
        it->setData("IpLogo.png", BuddyListItemModel::GenericAvatar);
    else
        it->setData("PcLogo.png", BuddyListItemModel::GenericAvatar);

    // Update logo
    if (platform.toLower() == "windows")
        it->setData("WindowsLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "macintosh")
        it->setData("AppleLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "linux")
        it->setData("LinuxLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "symbian")
        it->setData("SymbianLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "ios")
        it->setData("IosLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "windowsphone")
        it->setData("WindowsPhoneLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "blackberry")
        it->setData("BlackberryLogo.png", BuddyListItemModel::OsLogo);
    else if (platform.toLower() == "android")
        it->setData("AndroidLogo.png", BuddyListItemModel::OsLogo);
    else
        it->setData("UnknownLogo.png", BuddyListItemModel::OsLogo);

    // Add elemento to the list
    if (add) {
        appendRow(it);
        if (ip != "") mItemsMap.insert(ip, it);
    }
}

void BuddyListItemModel::addBuddy(Peer &peer)
{
    QRegExp rx("^(.*)\\sat\\s(.*)\\s\\((.*)\\)$");
    rx.indexIn(peer.name);
    QStringList data = rx.capturedTexts();

    QString username = data[1];
    QString system = data[2];
    QString platform = data[3];
    QUrl avatarPath = QUrl("http://" + peer.address.toString() + ":" + QString::number(peer.port + 1) + "/dukto/avatar");

    addBuddy(peer.address.toString(),
             peer.port,
             username,
             system,
             platform,
             avatarPath);
}

void BuddyListItemModel::removeBuddy(QString ip)
{
    // Check for element
    if (!mItemsMap.contains(ip)) return;

    // Get element
    QStandardItem* it = mItemsMap[ip];

    // Remove element
    mItemsMap.remove(ip);
    this->removeRow(this->indexFromItem(it).row());
}

void BuddyListItemModel::showSingleBack(int idx)
{
    for (int i = 0; i < rowCount(); i++)
        itemFromIndex(index(i, 0))->setData(false, BuddyListItemModel::ShowBack);
    itemFromIndex(index(idx, 0))->setData(true, BuddyListItemModel::ShowBack);
}

QString BuddyListItemModel::buddyNameByIp(QString ip)
{
    if (!mItemsMap.contains(ip)) return "";
    return mItemsMap.value(ip)->data(BuddyListItemModel::Username).toString();
}

QStandardItem* BuddyListItemModel::buddyByIp(QString ip)
{
    if (!mItemsMap.contains(ip)) return NULL;
    return mItemsMap.value(ip);
}

QString BuddyListItemModel::fistBuddyIp()
{
    if (this->rowCount() < 3) return "";
    return this->index(2, 0).data(BuddyListItemModel::Ip).toString();
}

