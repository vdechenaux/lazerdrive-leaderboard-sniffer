#include "lazerdriveapp.h"

LazerDriveApp::LazerDriveApp(QObject *parent) : QObject(parent)
{
    m_pPlayersCache = new QMap<uint, QLazerDrivePlayer>();

    m_pFlushTimer = new QTimer(this);
    connect(m_pFlushTimer, SIGNAL(timeout()), this, SLOT(flushTimerTimeout()));
    m_pFlushTimer->start(LazerDriveConfiguration::instance()->value("database/persist_interval", 30).toInt() * 1000);

    m_pClient = new QLazerDriveClient(this);

    connect(m_pClient, SIGNAL(connected(QLazerDrivePlayer)), this, SLOT(lazerdriveConnected(QLazerDrivePlayer)));
    connect(m_pClient, SIGNAL(leaderBoardLineReceived(uint,uint,uint,QString)), this, SLOT(lazerdriveLeaderBoardLineReceived(uint,uint,uint,QString)));
    connect(m_pClient, SIGNAL(leaderBoardScoreChanged(uint,uint)), this, SLOT(lazerdriveLeaderBoardScoreChanged(uint,uint)));
    connect(m_pClient, SIGNAL(playerEnteredTheGame(QLazerDrivePlayer,bool,bool)), this, SLOT(lazerdrivePlayerEnteredTheGame(QLazerDrivePlayer,bool,bool)));
    connect(m_pClient, SIGNAL(playerLeftTheGame(QLazerDrivePlayer,bool)), this, SLOT(lazerdrivePlayerLeftTheGame(QLazerDrivePlayer,bool)));
    connect(m_pClient, SIGNAL(existingPlayerInitialized(QLazerDrivePlayer,uint,uint)), this, SLOT(lazerdriveExistingPlayerInitialized(QLazerDrivePlayer,uint,uint)));

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

void LazerDriveApp::lazerdriveLeaderBoardLineReceived(uint playerId, uint score, uint rank, QString name)
{
    addPlayerToCache(playerId, name);
    updatePlayerScore(playerId, score);
}

void LazerDriveApp::lazerdriveLeaderBoardScoreChanged(uint playerId, uint score)
{
    updatePlayerScore(playerId, score);
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

void LazerDriveApp::updatePlayerScore(const uint &id, const uint &score)
{
    if (!m_pPlayersCache->contains(id))
        return;

    QLazerDrivePlayer player = m_pPlayersCache->value(id);
    player.setScore(score);
    m_pPlayersCache->insert(id, player);

    qDebug() << "[SCORE] Update player " << player.name() << score;
}

void LazerDriveApp::flushTimerTimeout()
{
    qDebug() << "[DATABASE] Flushing...";

    foreach (QLazerDrivePlayer player, *m_pPlayersCache) {
        LazerDriveDatabaseManager::instance()->updatePlayerHighScore(player.name(), player.score());
    }

    LazerDriveDatabaseManager::instance()->updateOnlinePlayers(playerNames());
}

QStringList LazerDriveApp::playerNames()
{
    QStringList names;
    foreach (QLazerDrivePlayer player, *m_pPlayersCache) {
        if (!names.contains(player.name())) {
            names << player.name();
        }
    }
    return names;
}
