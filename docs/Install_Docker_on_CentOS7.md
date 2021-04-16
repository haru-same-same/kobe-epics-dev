# Install Docker on CentOS7

## _This instruction is based on following websites_ :

- <https://zenn.dev/ymasaoka/articles/install-docker-centos-7>
- <https://hub.docker.com/r/pklaus/epics_base>

## Install Docker

```shell
$ sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
$ sudo yum install docker-ce docker-ce-cli containerd.io
```

## Run EPICS on Docker

### 1. Pull Docker image

``` shell
$ sudo docker pull pklaus/epics_base:3.15.6_debian
```

### 2. Run and start example IOC

Make sure that ports 5064/5065 are opened.

```shell
$ sudo docker run \
    --rm -it \
    -p 5064-5065:5064-5065 \
    -p 5064-5065:5064-5065/udp \
    -w /epics/iocs/example/iocBoot/iocEXAMPLE \
    pklaus/epics_base:3.15.6_debian \
    ./st.cmd
```

Options of _run_ command are described in [here](https://docs.docker.jp/engine/reference/commandline/run.html).