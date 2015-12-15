#include <QCoreApplication>
#include "lazerdriveapp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LazerDriveApp app;

    return a.exec();
}

