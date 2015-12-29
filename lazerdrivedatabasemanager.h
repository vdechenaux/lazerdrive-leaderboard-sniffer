#ifndef LAZERDRIVEDATABASEMANAGER_H
#define LAZERDRIVEDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QStringList>
#include <qlazerdriveplayer.h>
#include "lazerdriveconfiguration.h"

class LazerDriveDatabaseManager : public QObject
{
    Q_OBJECT
    static LazerDriveDatabaseManager *m_pInstance;
    QSqlDatabase m_database;
    explicit LazerDriveDatabaseManager(QObject *parent = 0);
    ~LazerDriveDatabaseManager();
public:
    void updatePlayer(const QLazerDrivePlayer &player);
    static LazerDriveDatabaseManager* instance();
    void updateOnlinePlayers(const QStringList &players);

signals:

public slots:
};

#endif // LAZERDRIVEDATABASEMANAGER_H
