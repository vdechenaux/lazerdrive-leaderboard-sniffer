#ifndef LAZERDRIVEAPP_H
#define LAZERDRIVEAPP_H

#include <QObject>
#include <qlazerdriveclient.h>
#include <QString>

class LazerDriveApp : public QObject
{
    Q_OBJECT
    QLazerDriveClient *m_pClient;
    QMap<uint, QLazerDrivePlayer> *m_pPlayersCache;
    void addPlayerToCache(const QLazerDrivePlayer &player);
    void addPlayerToCache(const uint &id, const QString &name);
    void removePlayerFromCache(const uint &id);
public:
    explicit LazerDriveApp(QObject *parent = 0);
    ~LazerDriveApp();

signals:

private slots:
    void lazerdriveConnected(QLazerDrivePlayer player);
    void lazerdriveLeaderBoardLineReceived(uint playerId, uint score, uint rank, QString name);
    void lazerdriveLeaderBoardScoreChanged(uint playerId, uint score);
    void lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias);
    void lazerdrivePlayerLeftTheGame(QLazerDrivePlayer player, bool isAlias);
    void lazerdriveExistingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y);
};

#endif // LAZERDRIVEAPP_H
