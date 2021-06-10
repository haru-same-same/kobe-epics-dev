# Builder image
FROM debian:10-slim

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV WORK_DIR=/usr/local/epics \
    EPICS_VER=3.15.8

# Update yum and install necessary packages
RUN apt update -y \
    && apt upgrade -y \
    && apt install -y \
    wget \
    build-essential \
    libreadline-dev \
    && apt clean

# Download and install epics
RUN mkdir ${WORK_DIR} && cd ${WORK_DIR} \
    && wget -q https://epics.anl.gov/download/base/base-${EPICS_VER}.tar.gz \
    && tar -zxvf base-${EPICS_VER}.tar.gz \
    && rm base-${EPICS_VER}.tar.gz \
    && cd base-${EPICS_VER} \
    && make

# Expose port
EXPOSE 5064-5065/tcp
EXPOSE 5064-5065/udp

CMD ["/bin/bash"]
