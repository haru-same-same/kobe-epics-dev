## Github pages

### How to contribute

このリポジトリの`docs/`以下は、Github Pagesによって[公開](https://haru-same-same.github.io/kobe-epics-dev/)されています。<br>

新たなページの追加や内容の追記を行いたい場合は、Pull requestして下さい。



## Docker containers

`docker/`以下には、EPICSを動かすためのDockerfileを置いています。<br>

これらの内、主要なものは[DockerHub](https://hub.docker.com/r/abes0/)にアップロードしてあるので、ビルド無しで使用できます。

### base

EPICS baseのみをインストールしているDockerイメージです。

```bash
$ docker run --rm -it abes0/epics-base:latest
```

### gateway

EPICS baseに加え、gatewayをインストールしたDockerイメージです。

``` bash
$ docker run --rm -it abes0/epics-gateway:latest
```

### base-cafe

EPICS baseに加え、[CAFE](http://cafe.psi.ch/)をインストールしたDockerイメージです。

```bash
$ docker run --rm -it abes0/epics-cafe:latest
```

CAFEを試すためのdocker-compose.ymlも準備しているので、以下のようにCAFEを試すことができます。

```bash
# terminal 1

$ cd docker/base-cafe
$ docker-compose up -d
$ docker exec -it base-cafe_base_1 /bin/bash
(container)$ ./st.cmd # test ioc起動

# terminal 2

$ docker exec -it base-cafe_cafe_1 /bin/bash
(container)$ make
(container)$ ./cafeTest
```



