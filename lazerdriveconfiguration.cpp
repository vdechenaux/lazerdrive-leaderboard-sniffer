#include "lazerdriveconfiguration.h"

LazerDriveConfiguration::LazerDriveConfiguration(QObject *parent) : QObject(parent)
{
    m_pSettings = new QSettings(m_path, QSettings::IniFormat, this);
}

QString LazerDriveConfiguration::m_path = "";
LazerDriveConfiguration* LazerDriveConfiguration::m_pInstance = 0;

LazerDriveConfiguration* LazerDriveConfiguration::instance()
{
    if (!m_pInstance) {
        m_pInstance = new LazerDriveConfiguration();
    }
    return m_pInstance;
}

QVariant LazerDriveConfiguration::value(const QString &key, const QVariant &defaultValue) const
{
    return m_pSettings->value(key, defaultValue);
}

bool LazerDriveConfiguration::init(const QString &absoluteFilePath)
{
    if (QFile::exists(absoluteFilePath)) {
        m_path = absoluteFilePath;
        return true;
    }

    return false;
}

