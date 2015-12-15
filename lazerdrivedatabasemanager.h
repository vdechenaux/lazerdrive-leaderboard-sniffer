#ifndef LAZERDRIVEDATABASEMANAGER_H
#define LAZERDRIVEDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QStringList>
#include "lazerdriveconfiguration.h"

class LazerDriveDatabaseManager : public QObject
{
    Q_OBJECT
    static LazerDriveDatabaseManager *m_pInstance;
    QSqlDatabase m_database;
    explicit LazerDriveDatabaseManager(QObject *parent = 0);
    ~LazerDriveDatabaseManager();
public:
    void updatePlayerHighScore(const QString &name, const uint &score);
    static LazerDriveDatabaseManager* instance();
    void updateOnlinePlayers(const QStringList &players);

signals:

public slots:
};

#endif // LAZERDRIVEDATABASEMANAGER_H
