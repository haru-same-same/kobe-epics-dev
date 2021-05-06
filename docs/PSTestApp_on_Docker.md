# PSTestApp on Docker

[Back to main page](../index.md)

## DockerHub repository

[abes0/psapptest](https://hub.docker.com/r/abes0/psapptest)

## Usage

### Run container

Make sure the ports 5064-5065 are open.

```shell
$ docker pull abes0/psapptest:latest
$ docker run -rm -it \
  -p 5064-5065:5064-5065 \
  -p 5064-5065:5064-5065/udp \
  abes0/psapptest:latest
```

### Add records to DB

First, clone git repository

```shell
$ git clone https://github.com/haru-same-same/kobe-epics-dev.git
```

Then, edit .db file and .proto file in 'kobe-epics-dev/docker/PSAppTest/PS_DB'

```shell
$ cd kobe-epics-dev/docker/PSAppTest/PS_DB
$ vim PS.db # can edit
$ vim tc-l2s.proto # can edit
```

Finally, build the image

```shell
$ docker build . -t [user]/[name]:[tag]
```

