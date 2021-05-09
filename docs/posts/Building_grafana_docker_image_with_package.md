# Building grafana docker image with package

## Clone grafana repository

```shell
$ git clone https://github.com/grafana/grafana.git
```

## Build the image

```shell
$ cd grafana/packaging/docker/custom
$docker buildx build \
 --build-arg "GRAFANA_VERSION=latest" \
 --build-arg "GF_INSTALL_PLUGINS=[plugin .zip file url];[plugin name]" \
 -t [user]/[repo]:[tag] \
 -f Dockerfile . \
 --load
```

