#ifndef LAZERDRIVECACHEENTRY_H
#define LAZERDRIVECACHEENTRY_H

#include <qlazerdriveplayer.h>

struct LazerDriveCacheEntry
{
    LazerDriveCacheEntry(QLazerDrivePlayer p = QLazerDrivePlayer(), uint deaths = 0, uint kills = 0)
    {
        player = p;
        deathsCount = deaths;
        killsCount = kills;
    }
    QLazerDrivePlayer player;
    uint deathsCount, killsCount;
};

#endif // LAZERDRIVECACHEENTRY_H
