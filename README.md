# Lazerdrive leaderboard sniffer

[![Build Status](https://travis-ci.org/vdechenaux/lazerdrive-leaderboard-sniffer.svg?branch=master)](https://travis-ci.org/vdechenaux/lazerdrive-leaderboard-sniffer)

This tool is based on my library: [QLazerDriveClient](https://github.com/vdechenaux/QLazerDriveClient).

## How to use it

You must have a Qt5 sdk installed (packages `qt5-qmake qt5-default`) with websockets (`libqt5websockets5-dev`) and mysql support (`libqt5sql5-mysql`). To build it, you need the `build-essential` package.
```
git submodule update --init
cd QLazerDriveClient
qmake
make
make install
cd ..
qmake
make
./lazerdrive-leaderboard-sniffer
```
You can also use Qt Creator if you are not familiar with a shell.

## How to use it (with Docker)

```
# Build
git submodule update --init
docker build -t lazerdrive-leaderboard-sniffer .

# Run
docker run --rm=true -t lazerdrive-leaderboard-sniffer

# Run daemonized:
docker run -dt lazerdrive-leaderboard-sniffer
```

To connect to a MySql server within Docker, you should take a look at the `--link` Docker option.

## CLI arguments
```
Usage: lazerdrive-leaderboard-sniffer [options]

Options:
  -?, -h, --help             Displays this help.
  -v, --version              Displays version information.
  -c, --config <file>        Load config from ini file <file>.
  --verbose <level>          Set the verbose level.
                             Available levels:
                             0 quiet mode, equivalent to -q or --quiet
                             1 only displays errors
                             2 displays errors and warnings
                             3 (default) displays errors, warnings and infos
                             4 displays all messages, including debug
  -q, --quiet                Run in silent mode, equivalent to --verbose 0.
  -s, --server <host>        Connect to this server. The default value is "one.eu.lazerdrive.io".
  -u, --username <username>  Use this username. If you don't specify it, you will have a random username.
```

You want to connect to server "three" with "toto" as username? Just do this:
```
lazerdrive-leaderboard-sniffer --server three.eu.lazerdrive.io --username toto
# Or if you use Docker:
docker run --rm=true -t lazerdrive-leaderboard-sniffer --server three.eu.lazerdrive.io --username toto
```
## Config.ini
To custom parameters like database credentials, you must use an ini file with `--config` option.
Here is an example of config:
```
[database]
host=localhost
dbname=lazerdrive
user=root
pass=toor
persist_interval=30
```

If you want to use a `config.ini` with the Docker image, you must put it in the same directory as the Dockerfile and re-build the image. The file will be copied into it.
