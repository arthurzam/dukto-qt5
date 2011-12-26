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

#ifndef DUKTOPROTOCOL_H
#define DUKTOPROTOCOL_H

#include <QObject>

#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostInfo>
#include <QHash>
#include <QFile>

#include "peer.h"

class DuktoProtocol : public QObject
{
    Q_OBJECT

public:
    DuktoProtocol();
    virtual ~DuktoProtocol();
    void initialize();
    void setPorts(qint16 udp, qint16 tcp);
    void sayHello(QHostAddress dest);
    void sayHello(QHostAddress dest, qint16 port);
    void sayGoodbye();
    inline QHash<QString, Peer>& getPeers() { return mPeers; }
    void sendFile(QString ipDest, qint16 port, QStringList files);
    void sendText(QString ipDest, qint16 port, QString text);
    inline bool isBusy() { return mIsSending || mIsReceiving; }
    void abortCurrentTransfer();
    
public slots:
    void newUdpData();
    void newIncomingConnection();
    void readNewData();
    void closedConnection();
    void closedConnectionTmp();
    void sendMetaData();
    void sendData(qint64 b);
    void sendConnectError(QAbstractSocket::SocketError);

signals:
     void peerListAdded(Peer peer);
     void peerListRemoved(Peer peer);
     void sendFileComplete(QStringList *files);
     void sendFileError(int code);
     void sendFileAborted();
     void receiveFileStart(QString senderIp);
     void receiveFileComplete(QStringList *files, qint64 totalSize);
     void receiveTextComplete(QString *text, qint64 totalSize);
     void receiveFileCancelled();
     void transferStatusUpdate(qint64 total, qint64 partial);

private:
    QString getSystemSignature();
    QStringList* expandTree(QStringList files);
    void addRecursive(QStringList *e, QString path);
    qint64 computeTotalSize(QStringList *e);
    QByteArray nextElementHeader();
    void sendToAllBroadcast(QByteArray *packet, qint16 port);
    void closeCurrentTransfer(bool aborted = false);

    void handleMessage(QByteArray &data, QHostAddress &sender);
    void updateStatus();

    QUdpSocket *mSocket;            // Socket UDP segnalazione
    QTcpServer *mTcpServer;         // Socket TCP attesa dati
    QTcpSocket *mCurrentSocket;     // Socket TCP dell'attuale trasferimento file

    QHash<QString, Peer> mPeers;    // Elenco peer individuati

    // Send and receive members
    qint16 mLocalUdpPort;
    qint16 mLocalTcpPort;
    bool mIsSending;
    bool mIsReceiving;
    QFile *mCurrentFile;            // Puntatore al file aperto corrente
    qint64 mTotalSize;              // Quantit� totale di dati da inviare o ricevere
    int mFileCounter;              // Puntatore all'elemento correntemente da trasmettere o ricevere

    // Sending members
    QStringList *mFilesToSend;      // Elenco degli elementi da trasmettere
    qint64 mSentData;               // Quantit� di dati totale trasmessi
    qint64 mSentBuffer;             // Quantit� di dati rimanenti nel buffer di trasmissione
    QString mBasePath;              // Percorso base per l'invio di file e cartelle
    QString mTextToSend;            // Testo da inviare (in caso di invio testuale)

    // Receive members
    qint64 mElementsToReceiveCount;    // Numero di elementi da ricevere
    qint64 mTotalReceivedData;         // Quantit� di dati ricevuti totale
    qint64 mElementReceivedData;       // Quantit� di dati ricevuti per l'elemento corrente
    qint64 mElementSize;               // Dimensione dell'elemento corrente
    QString mRootFolderName;           // Nome della cartella principale ricevuta
    QString mRootFolderRenamed;        // Nome della cartella principale da utilizzare
    QStringList *mReceivedFiles;        // Elenco degli elementi da trasmettere
    QByteArray mTextToReceive;             // Testo ricevuto in caso di invio testo
    bool mReceivingText;               // Ricezione di testo in corso
    QByteArray mPartialName;              // Nome prossimo file letto solo in parte

};

#endif // DUKTOPROTOCOL_H
