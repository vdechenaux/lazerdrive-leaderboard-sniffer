#include "lazerdriveapp.h"

LazerDriveApp::LazerDriveApp(QObject *parent) : QObject(parent)
{
    m_pPlayersCache = new QMap<uint, QLazerDrivePlayer>();

    m_pClient = new QLazerDriveClient(this);

    connect(m_pClient, SIGNAL(connected(QLazerDrivePlayer)), this, SLOT(lazerdriveConnected(QLazerDrivePlayer)));
    connect(m_pClient, SIGNAL(leaderBoardLineReceived(uint,uint,uint,QString)), this, SLOT(lazerdriveLeaderBoardLineReceived(uint,uint,uint,QString)));
    connect(m_pClient, SIGNAL(leaderBoardScoreChanged(uint,uint)), this, SLOT(lazerdriveLeaderBoardScoreChanged(uint,uint)));
    connect(m_pClient, SIGNAL(playerEnteredTheGame(QLazerDrivePlayer,bool,bool)), this, SLOT(lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer,bool,bool)));
    connect(m_pClient, SIGNAL(playerLeftTheGame(QLazerDrivePlayer,bool)), this, SLOT(lazerdrivePlayerLeftTheGame(QLazerDrivePlayer,bool)));
    connect(m_pClient, SIGNAL(existingPlayerInitialized(QLazerDrivePlayer,uint,uint)), this, SLOT(lazerdriveExistingPlayerInitialized(QLazerDrivePlayer,uint,uint)));

    m_pClient->connectToServer("one.eu.lazerdrive.io", "sniffer");
}

LazerDriveApp::~LazerDriveApp()
{
    delete m_pPlayersCache;
}

void LazerDriveApp::lazerdriveConnected(QLazerDrivePlayer player)
{
    addPlayerToCache(player);
}

void LazerDriveApp::lazerdriveLeaderBoardLineReceived(uint playerId, uint score, uint rank, QString name)
{
    addPlayerToCache(playerId, name);
}

void LazerDriveApp::lazerdriveLeaderBoardScoreChanged(uint playerId, uint score)
{

}

void LazerDriveApp::lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias)
{
    addPlayerToCache(player);
}

void LazerDriveApp::lazerdrivePlayerLeftTheGame(QLazerDrivePlayer player, bool isAlias)
{
    removePlayerFromCache(player.id());
}

void LazerDriveApp::lazerdriveExistingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y)
{
    addPlayerToCache(player);
}

void LazerDriveApp::addPlayerToCache(const QLazerDrivePlayer &player)
{
    if (!m_pPlayersCache->contains(player.id())) {
        m_pPlayersCache->insert(player.id(), player);
        qDebug() << "[CACHE] Add player " << player.name();
    }
}

void LazerDriveApp::addPlayerToCache(const uint &id, const QString &name)
{
    QLazerDrivePlayer player;
    player.setId(id);
    player.setName(name);
    addPlayerToCache(player);
}

void LazerDriveApp::removePlayerFromCache(const uint &id)
{
    if (m_pPlayersCache->remove(id)) {
        qDebug() << "[CACHE] Remove player Id " << id;
    } else {
        qWarning() << "[CACHE] Trying to remove a non cached player, skipping it. Id " << id;
    }
}
