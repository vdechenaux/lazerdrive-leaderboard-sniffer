#ifndef LAZERDRIVEAPP_H
#define LAZERDRIVEAPP_H

#include <QObject>
#include <qlazerdriveclient.h>
#include <QTimer>
#include "lazerdrivedatabasemanager.h"

class LazerDriveApp : public QObject
{
    Q_OBJECT
    QLazerDriveClient *m_pClient;
    QMap<uint, QLazerDrivePlayer> *m_pPlayersCache;
    void addPlayerToCache(const QLazerDrivePlayer &player);
    void removePlayerFromCache(const uint &id);
    void updatePlayerScore(const uint &id, const uint &score);
    QTimer *m_pFlushTimer;
public:
    explicit LazerDriveApp(QObject *parent = 0);
    ~LazerDriveApp();
    QStringList playerNames();

signals:

private slots:
    void lazerdriveConnected(QLazerDrivePlayer player);
    void lazerdriveLeaderBoardLineReceived(QLazerDrivePlayer player, uint rank);
    void lazerdriveLeaderBoardScoreChanged(uint playerId, uint score);
    void lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias);
    void lazerdrivePlayerLeftTheGame(QLazerDrivePlayer player, bool isAlias);
    void lazerdriveExistingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y);
    void flushTimerTimeout();
};

#endif // LAZERDRIVEAPP_H
