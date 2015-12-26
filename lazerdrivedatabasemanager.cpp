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

void LazerDriveDatabaseManager::updatePlayerHighScore(const QString &name, const uint &score)
{
    if (!m_database.isOpen()) {
        qWarning() << "Skip score update because the connection to the database is not valid.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT score FROM highscore WHERE player = :name");
    query.bindValue(":name", name.toUtf8());
    query.exec();

    if (query.size() == 0) {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO highscore (score, player) VALUES (:score, :name)");
        insertQuery.bindValue(":score", score);
        insertQuery.bindValue(":name", name.toUtf8());
        insertQuery.exec();
    }
    else if (query.first() && score > query.value(0).toInt()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE highscore SET score = :score WHERE player = :name");
        updateQuery.bindValue(":score", score);
        updateQuery.bindValue(":name", name.toUtf8());
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
    query.exec("UPDATE highscore SET online = 0");

    if (!players.isEmpty()) {
        QSqlQuery updateQuery;

        QString sql = "UPDATE highscore SET online = 1 WHERE player IN (";

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
