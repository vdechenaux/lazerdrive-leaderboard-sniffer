#include "lazerdriveapp.h"

LazerDriveApp::LazerDriveApp(QObject *parent) : QObject(parent)
{
    m_pPlayersCache = new QMap<uint, LazerDriveCacheEntry>();

    m_pFlushTimer = new QTimer(this);
    connect(m_pFlushTimer, SIGNAL(timeout()), this, SLOT(flushTimerTimeout()));
    m_pFlushTimer->start(LazerDriveConfiguration::instance()->value("database/persist_interval", 30).toInt() * 1000);

    m_pClient = new QLazerDriveClient(this);

    connect(m_pClient, SIGNAL(connected(QLazerDrivePlayer)), this, SLOT(lazerdriveConnected(QLazerDrivePlayer)));
    connect(m_pClient, SIGNAL(leaderBoardLineReceived(QLazerDrivePlayer,uint)), this, SLOT(lazerdriveLeaderBoardLineReceived(QLazerDrivePlayer,uint)));
    connect(m_pClient, SIGNAL(leaderBoardScoreChanged(uint,uint)), this, SLOT(lazerdriveLeaderBoardScoreChanged(uint,uint)));
    connect(m_pClient, SIGNAL(playerEnteredTheGame(QLazerDrivePlayer,bool,bool)), this, SLOT(lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer,bool,bool)));
    connect(m_pClient, SIGNAL(playerLeftTheGame(QLazerDrivePlayer,bool)), this, SLOT(lazerdrivePlayerLeftTheGame(QLazerDrivePlayer,bool)));
    connect(m_pClient, SIGNAL(existingPlayerInitialized(QLazerDrivePlayer,uint,uint)), this, SLOT(lazerdriveExistingPlayerInitialized(QLazerDrivePlayer,uint,uint)));
    connect(m_pClient, SIGNAL(playerDead(uint,uint,QLazerDrivePlayer::DeathTypes,uint,uint,uint)), this, SLOT(lazerdrivePlayerDead(uint,uint,QLazerDrivePlayer::DeathTypes,uint,uint,uint)));

    m_pClient->connectToServer(LazerDriveConfiguration::instance()->property("lazerDriveServer").toString(), LazerDriveConfiguration::instance()->property("lazerDriveUsername").toString());
}

LazerDriveApp::~LazerDriveApp()
{
    delete m_pPlayersCache;
}

void LazerDriveApp::lazerdriveConnected(QLazerDrivePlayer player)
{
    addPlayerToCache(player);
    qInfo() << "Connected " << player.name();
}

void LazerDriveApp::lazerdriveLeaderBoardLineReceived(QLazerDrivePlayer player, uint rank)
{
    Q_UNUSED(rank);
    addPlayerToCache(player);
}

void LazerDriveApp::lazerdriveLeaderBoardScoreChanged(uint playerId, uint score)
{
    updatePlayerScore(playerId, score);
}

void LazerDriveApp::lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer player, bool isMyself, bool isAlias)
{
    Q_UNUSED(isMyself);
    Q_UNUSED(isAlias);
    addPlayerToCache(player);
}

void LazerDriveApp::lazerdrivePlayerLeftTheGame(QLazerDrivePlayer player, bool isAlias)
{
    Q_UNUSED(isAlias);
    removePlayerFromCache(player.id());
}

void LazerDriveApp::lazerdriveExistingPlayerInitialized(QLazerDrivePlayer player, uint x, uint y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    addPlayerToCache(player);
}

void LazerDriveApp::lazerdrivePlayerDead(uint playerId, uint killerId, QLazerDrivePlayer::DeathTypes type, uint x, uint y, uint angle)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(angle);

    if (m_pPlayersCache->contains(playerId)) {
        LazerDriveCacheEntry victim = m_pPlayersCache->value(playerId);
        victim.deathsCount++;
        m_pPlayersCache->insert(playerId, victim);
    }

    if (type == QLazerDrivePlayer::Kill && m_pPlayersCache->contains(killerId)) {
        LazerDriveCacheEntry killer = m_pPlayersCache->value(killerId);
        killer.killsCount++;
        m_pPlayersCache->insert(killerId, killer);
    }
}

void LazerDriveApp::addPlayerToCache(const QLazerDrivePlayer &player)
{
    if (!m_pPlayersCache->contains(player.id())) {
        m_pPlayersCache->insert(player.id(), LazerDriveCacheEntry(player));
        qDebug() << "[CACHE] Add player " << player.name();
    } else {
        qDebug() << "[CACHE] Update player " << player.name();
        LazerDriveCacheEntry cached = m_pPlayersCache->value(player.id());
        cached.player.setR(player.r());
        cached.player.setG(player.g());
        cached.player.setB(player.b());
        m_pPlayersCache->insert(player.id(), cached);
        updatePlayerScore(player.id(), player.score());
    }
}

void LazerDriveApp::removePlayerFromCache(const uint &id)
{
    if (m_pPlayersCache->remove(id)) {
        qDebug() << "[CACHE] Remove player Id " << id;
    } else {
        qWarning() << "[CACHE] Trying to remove a non cached player, skipping it. Id " << id;
    }
}

void LazerDriveApp::updatePlayerScore(const uint &id, const uint &score)
{
    if (!m_pPlayersCache->contains(id))
        return;

    LazerDriveCacheEntry cached = m_pPlayersCache->value(id);
    if (score > cached.player.score()) {
        cached.player.setScore(score);
        m_pPlayersCache->insert(id, cached);
    }

    qDebug() << "[SCORE] Update player " << cached.player.name() << score;
}

void LazerDriveApp::flushTimerTimeout()
{
    qDebug() << "[DATABASE] Flushing...";

    foreach (LazerDriveCacheEntry cached, *m_pPlayersCache) {
        LazerDriveDatabaseManager::instance()->updatePlayer(cached);
    }

    LazerDriveDatabaseManager::instance()->updateOnlinePlayers(playerNames());
}

QStringList LazerDriveApp::playerNames()
{
    QStringList names;
    foreach (LazerDriveCacheEntry cached, *m_pPlayersCache) {
        if (!names.contains(cached.player.name())) {
            names << cached.player.name();
        }
    }
    return names;
}
