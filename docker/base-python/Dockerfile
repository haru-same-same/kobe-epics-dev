# Builder image
FROM abes0/epics-base:latest

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV EPICS_VER=3.15.8 \
    EPICS_HOST_ARCH=linux-x86_64 \
    WORK_DIR=/usr/local/epics

WORKDIR /

# Update yum and install necessary packages
RUN apt update -y && apt upgrade -y && apt install -y python3 python3-pip \
    && pip3 install wheel pyepics pymysql websocket_server
