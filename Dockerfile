FROM debian:8

RUN echo "deb http://httpredir.debian.org/debian unstable main" >> /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -t unstable -y build-essential qt5-qmake qt5-default libqt5websockets5-dev libqt5sql5-mysql

RUN mkdir /code
COPY . /code/
WORKDIR /code/QLazerDriveClient
RUN qmake
RUN make
RUN make install
WORKDIR /code
RUN qmake
RUN make

ENTRYPOINT ["/code/lazerdrive-leaderboard-sniffer"]
