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

#include "duktowindow.h"
#include "guibehind.h"
#include "platform.h"
#include "settings.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

DuktoWindow::DuktoWindow(QWidget *parent) :
    QmlApplicationViewer(parent), mGuiBehind(NULL)
{
    // Configure window
    setAcceptDrops(true);
    setWindowTitle("Dukto R5");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setMaximumSize(350, 5000);
    setMinimumSize(350, 500);
    setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

    // Taskbar integration with Win7
    mWin7.init(this->winId());
}

#ifdef Q_WS_WIN
bool DuktoWindow::winEvent(MSG * message, long * result)
{
    return mWin7.winEvent(message, result);
}
#endif

void DuktoWindow::setGuiBehindReference(GuiBehind* ref)
{
    mGuiBehind = ref;
}

void DuktoWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && mGuiBehind->canAcceptDrop())
        event->acceptProposedAction();
}

void DuktoWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DuktoWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void DuktoWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls()) return;

    QStringList files;
    const QList<QUrl> urlList = mimeData->urls();
    foreach (const QUrl &url, urlList)
        files.append(url.toLocalFile());

       event->acceptProposedAction();
    mGuiBehind->sendDroppedFiles(&files);
}

void DuktoWindow::closeEvent(QCloseEvent *event)
{
    mGuiBehind->settings()->saveWindowGeometry(saveGeometry());
    mGuiBehind->close();
}
