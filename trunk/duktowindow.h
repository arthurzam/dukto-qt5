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
 
#ifndef DUKTOWINDOW_H
#define DUKTOWINDOW_H

#include "qmlapplicationviewer/qmlapplicationviewer.h"
#include "ecwin7.h"

class GuiBehind;

class DuktoWindow : public QmlApplicationViewer
{
    Q_OBJECT

public:
    explicit DuktoWindow(QWidget *parent = 0);
    void setGuiBehindReference(GuiBehind* ref);
    inline EcWin7* win7() { return &mWin7; }

protected:
#ifdef Q_WS_WIN
    bool winEvent(MSG * message, long * result);
#endif
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);

signals:

public slots:

private:
    GuiBehind *mGuiBehind;
    EcWin7 mWin7;
};

#endif // DUKTOWINDOW_H
