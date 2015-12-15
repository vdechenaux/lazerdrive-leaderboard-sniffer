#ifndef LAZERDRIVECONFIGURATION_H
#define LAZERDRIVECONFIGURATION_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QFile>

class LazerDriveConfiguration : public QObject
{
    Q_OBJECT
    static LazerDriveConfiguration *m_pInstance;
    explicit LazerDriveConfiguration(QObject *parent = 0);
    QSettings *m_pSettings;
    static QString m_path;
public:
    static LazerDriveConfiguration* instance();
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    static bool init(const QString &absoluteFilePath);

signals:

public slots:
};

#endif // LAZERDRIVECONFIGURATION_H
