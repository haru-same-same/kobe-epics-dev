# Install Docker on CentOS7

[Back to main page](../index.md)

## _This instruction is based on following websites_ :

- <https://zenn.dev/ymasaoka/articles/install-docker-centos-7>
- <https://hub.docker.com/r/pklaus/epics_base>

## Install Docker

```shell
$ sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
$ sudo yum install docker-ce docker-ce-cli containerd.io
```

## Run EPICS softIOC on Docker

### 1. Pull GitHub repository

Please pull the repository. It contains example db file which is used in next step.

``` shell
$ git pull https://github.com/haru-same-same/kobe-epics-dev.git
```

### 2. Run and start example IOC

Make sure that ports 5064/5065 are opened.

Run the newest softIOC docker image from [docker hub](). This image requires db file, and we can use example db file in repository.

```shell
$ cd kobe-epics-dev/docker
$ sudo docker run --rm -it \
  -v $(pwd)/../db/:/db \
  -p 5064-5065:5064-5065 \
  -p 5064-5065:5064-5065/udp \
  abes0/kobe-bl-epics:softioc-latest
  
  Starting iocInit
############################################################################
## EPICS R3.15.8
## EPICS Base built May  4 2021
############################################################################
iocRun: All initialization complete
epics> dbl
test:count
```

Options of _run_ command are described in [here](https://docs.docker.jp/engine/reference/commandline/run.html).

Or you can use docker-compose instead of 'docker run' command.

```shell
$ cd kobe-epics-dev/docker/softIOC
$ docker-compose up
```



