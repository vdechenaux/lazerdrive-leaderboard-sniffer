#include "lazerdrivedatabasemanager.h"

LazerDriveDatabaseManager::LazerDriveDatabaseManager(QObject *parent) : QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName(LazerDriveConfiguration::instance()->value("database/host", "localhost").toString());
    m_database.setPort(LazerDriveConfiguration::instance()->value("database/port", 3306).toInt());
    m_database.setDatabaseName(LazerDriveConfiguration::instance()->value("database/dbname", "lazerdrive").toString());
    m_database.setUserName(LazerDriveConfiguration::instance()->value("database/user", "root").toString());
    m_database.setPassword(LazerDriveConfiguration::instance()->value("database/pass").toString());
    if (!m_database.open()) {
        qWarning() << "Can't connect to MySql database. " << m_database.lastError().text();
    }
}

LazerDriveDatabaseManager::~LazerDriveDatabaseManager()
{
    m_database.close();
}

void LazerDriveDatabaseManager::updatePlayer(const QLazerDrivePlayer &player)
{
    if (!m_database.isOpen()) {
        qWarning() << "Skip score update because the connection to the database is not valid.";
        return;
    }

    QString hexColor = QString::number((player.r() << 16) + (player.g() << 8) + player.b(), 16);
    if (hexColor.length() == 5) {
        hexColor.prepend('0');
    }

    QSqlQuery query;
    query.prepare("SELECT id, highscore, color FROM player WHERE name = :name");
    query.bindValue(":name", player.name().toUtf8());
    query.exec();

    if (query.size() == 0) {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO player (name, color, highscore, is_online) VALUES (:name, :color, :score, 0)");
        insertQuery.bindValue(":name", player.name().toUtf8());
        insertQuery.bindValue(":color", hexColor);
        insertQuery.bindValue(":score", player.score());
        insertQuery.exec();
    }
    else if (query.first() && (player.score() > query.value("highscore").toUInt() || hexColor != query.value("color").toString())) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE player SET highscore = GREATEST(:scoreLast, :scoreNow), color = :color WHERE id = :id");
        updateQuery.bindValue(":scoreLast", query.value("highscore").toUInt());
        updateQuery.bindValue(":scoreNow", player.score());
        updateQuery.bindValue(":color", hexColor);
        updateQuery.bindValue(":id", query.value("id").toInt());
        updateQuery.exec();
    }
}

void LazerDriveDatabaseManager::updateOnlinePlayers(const QStringList &players)
{
    if (!m_database.isOpen()) {
        qWarning() << "Skip online players update because the connection to the database is not valid.";
        return;
    }

    QSqlQuery query;
    query.exec("UPDATE player SET is_online = 0");

    if (!players.isEmpty()) {
        QSqlQuery updateQuery;

        QString sql = "UPDATE player SET is_online = 1 WHERE name IN (";

        sql.append(QString("?,").repeated(players.length()));
        sql.replace(sql.length()-1 , 1, ")");

        updateQuery.prepare(sql);

        foreach (QString name, players) {
            updateQuery.addBindValue(name.toUtf8());
        }

        updateQuery.exec();
    }
}

LazerDriveDatabaseManager* LazerDriveDatabaseManager::m_pInstance = 0;

LazerDriveDatabaseManager* LazerDriveDatabaseManager::instance()
{
    if (!m_pInstance) {
        m_pInstance = new LazerDriveDatabaseManager();
    }
    return m_pInstance;
}
