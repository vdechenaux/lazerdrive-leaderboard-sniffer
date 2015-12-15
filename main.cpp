#include <QCoreApplication>
#include "lazerdriveapp.h"
#include <QCommandLineParser>
#include <QFileInfo>
#include <QDir>

void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    int verboseLevel = qApp->property("lazerDriveVerboseLevel").toInt();

    if (type == QtFatalMsg && verboseLevel >= 1) {
        if (verboseLevel >= 1) {
            fprintf(stderr, "[FATAL]    %s\n", msg.toLocal8Bit().constData());
        }
        abort();
    }

    if (type == QtDebugMsg && verboseLevel >= 4) {
        fprintf(stderr, "[DEBUG]    %s\n", msg.toLocal8Bit().constData());
    } else if (type == QtInfoMsg && verboseLevel >= 2) {
        fprintf(stderr, "[INFO]     %s\n", msg.toLocal8Bit().constData());
    } else if (type == QtWarningMsg && verboseLevel >= 1) {
        fprintf(stderr, "[WARNING]  %s\n", msg.toLocal8Bit().constData());
    } else if (type == QtCriticalMsg && verboseLevel >= 1) {
        fprintf(stderr, "[CRITICAL] %s\n", msg.toLocal8Bit().constData());
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("lazerdrive-leaderboard-sniffer");
    QCoreApplication::setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configFileOption(
        QStringList() << "c" << "config",
        "Load config from ini file <file>.",
        "file"
    );
    parser.addOption(configFileOption);

    QCommandLineOption verboseOption(
        "verbose",
        "Set the verbose level.\nAvailable levels:\n0 quiet mode, equivalent to -q or --quiet\n1 only displays errors\n2 displays errors and warnings\n3 (default) displays errors, warnings and infos\n4 displays all messages, including debug",
        "level",
        "3"
    );
    parser.addOption(verboseOption);

    QCommandLineOption quietOption(
        QStringList() << "q" << "quiet",
        "Run in silent mode, equivalent to --verbose 0."
    );
    parser.addOption(quietOption);

    QCommandLineOption serverOption(
        QStringList() << "s" << "server",
        "Connect to this server. The default value is \"one.eu.lazerdrive.io\".",
        "host",
        "one.eu.lazerdrive.io"
    );
    parser.addOption(serverOption);

    QCommandLineOption usernameOption(
        QStringList() << "u" << "username",
        "Use this username. If you don't specify it, you will have a random username.",
        "username"
    );
    parser.addOption(usernameOption);

    parser.process(a);

    if (parser.isSet(quietOption)) {
        a.setProperty("lazerDriveVerboseLevel", 0);
    } else {
        a.setProperty("lazerDriveVerboseLevel", parser.value(verboseOption));
    }

    if (parser.isSet(configFileOption)) {
        QString configFile = parser.value(configFileOption);

        if (QFileInfo(configFile).isRelative()) {
            configFile = QDir::currentPath() + "/" + configFile;
        }

        if (!LazerDriveConfiguration::init(configFile)) {
            qWarning() << "Can't load provided configuration, falling back to default settings";
        }
    }

    LazerDriveConfiguration::instance()->setProperty("lazerDriveServer", parser.value(serverOption));
    LazerDriveConfiguration::instance()->setProperty("lazerDriveUsername", parser.value(usernameOption));

    LazerDriveApp app;

    return a.exec();
}

