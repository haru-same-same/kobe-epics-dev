# Builder image
FROM centos:7.9.2009 AS base

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV EPICS_VER=3.15.8 \
    ASYN_VER=4-38 \
    EPICS_HOST_ARCH=linux-x86_64 \
    WORK_DIR=/usr/local/epics \
    MOD_DIR=${EPICS_BASE}/modules \
    STREAM_DIR=${MOD_DIR}/StreamDevice \
    APP_DIR=/PSApp

# Update yum and install necessary packages
RUN yum update -y && yum install -y \
    wget \
    gcc-c++ \
    git \
    readline-devel \
    perl-devel \
    make

# Download and install epics base
RUN mkdir ${WORK_DIR} && cd $_ \
    && wget -q https://epics.anl.gov/download/base/base-${EPICS_VER}.tar.gz \
    && tar -xf base-${EPICS_VER}.tar.gz \
    && rm base-${EPICS_VER}.tar.gz \
    && cd base-${EPICS_VER} \
    && make

# Download and install Asyn driver
RUN mkdir ${WORK_DIR}/base-${EPICS_VER}/modules && cd $_ \
    && wget -q https://epics.anl.gov/download/modules/asyn${ASYN_VER}.tar.gz \
    && tar -xf asyn${ASYN_VER}.tar.gz \
    && rm asyn${ASYN_VER}.tar.gz \
    && echo "SUPPORT=${WORK_DIR}/base-${EPICS_VER}/modules" > asyn${ASYN_VER}/configure/RELEASE.local \
    && echo "EPICS_BASE=${WORK_DIR}/base-${EPICS_VER}" >> asyn${ASYN_VER}/configure/RELEASE.local \
    && sed -i "s|SNCSEQ=\$(SUPPORT)/seq-2-2-5|#SNCSEQ=\$(SUPPORT)/seq-2-2-5|g" asyn${ASYN_VER}/configure/RELEASE \
    && cd asyn${ASYN_VER} \
    && make

# Download and install StreamDevice
RUN cd ${WORK_DIR}/base-${EPICS_VER}/modules \
    && git clone https://github.com/paulscherrerinstitute/StreamDevice.git \
    && echo "ASYN=${WORK_DIR}/base-${EPICS_VER}/modules/asyn${ASYN_VER}" > StreamDevice/configure/RELEASE.local \
    && echo "EPICS_BASE=${WORK_DIR}/base-${EPICS_VER}" >> StreamDevice/configure/RELEASE.local \
    && sed -i "s|CALC=\$(SUPPORT)/calc-3-7|#CALC=\$(SUPPORT)/calc-3-7|g" StreamDevice/configure/RELEASE \
    && sed -i "s|PCRE=\$(SUPPORT)/pcre-7-2|#PCRE=\$(SUPPORT)/pcre-7-2|g" StreamDevice/configure/RELEASE \
    && cd StreamDevice \
    && make

# Expose port
EXPOSE 5064-5065/tcp
EXPOSE 5064-5065/udp

# Execute command
ENTRYPOINT ["/bin/bash"]
