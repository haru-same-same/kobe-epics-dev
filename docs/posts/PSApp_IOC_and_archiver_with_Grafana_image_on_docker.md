# PSApp IOC and archiver with Grafana image on docker

## _This instruction is based on following websites_ :

- <https://github.com/pklaus/archiver-appliance-with-example-ioc>
- <https://github.com/sasaki77/archiverappliance-datasource>

## 1. Setting up an image

### Install docker and docker-compose

Please refer to [this post](Install_Docker_on_CentOS7.md)

### Clone github repository

```shell
$ git clone https://github.com/haru-same-same/kobe-epics-dev.git
```

### Setting up environments

```shell
$ cd kobe-epics-dev/docker/ps-aa-grafana
$ chown 472:472 grafana-storage
```

## 2. Start the image

```shell
$ docker-compose up
```

