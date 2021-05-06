# Builder image
FROM --platform=${TARGETPLATFORM} centos:7.9.2009 AS base

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Images for multi platform
FROM base AS builder-amd64
ENV EPICS_HOST_ARCH=linux-x86_64

FROM base AS builder-arm64
ENV EPICS_HOST_ARCH=linux-arm

# Choose builder
FROM builder-${TARGETARCH} AS builder

# Setting variables
ENV EPICS_VER=3.15.8 \
    ASYN_VER=4-38 \
    USER=epics-dk \
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

# Copy Packages in order to use them in busybox
RUN mkdir /pkgs \
    && cp --parents /lib64/libstdc++.so.6 /pkgs \
    && cp --parents /lib64/libm.so.6 /pkgs \
    && cp --parents /lib64/libgcc_s.so.1 /pkgs \
    && cp --parents /lib64/libreadline.so.6 /pkgs \
    && cp --parents /lib64/librt.so.1 /pkgs \
    && cp --parents /lib64/libdl.so.2 /pkgs \
    && cp --parents /lib64/libtinfo.so.5 /pkgs

# Download and install epics base
RUN mkdir ${WORK_DIR} && cd $_ \
    && wget -q https://epics.anl.gov/download/base/base-${EPICS_VER}.tar.gz \
    && tar -zxvf base-${EPICS_VER}.tar.gz \
    && rm base-${EPICS_VER}.tar.gz \
    && cd base-${EPICS_VER} \
    && make

# Download and install Asyn driver
RUN mkdir ${WORK_DIR}/base-${EPICS_VER}/modules && cd $_ \
    && wget -q https://epics.anl.gov/download/modules/asyn${ASYN_VER}.tar.gz \
    && tar -zxvf asyn${ASYN_VER}.tar.gz \
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

# Create IOC App
RUN mkdir ${APP_DIR} && cd $_ \
    && ${WORK_DIR}/base-${EPICS_VER}/bin/${EPICS_HOST_ARCH}/makeBaseApp.pl -t ioc PSClient \
    && ${WORK_DIR}/base-${EPICS_VER}/bin/${EPICS_HOST_ARCH}/makeBaseApp.pl -i -t ioc PSClient \
    && echo \
    && echo "ASYN=${WORK_DIR}/base-${EPICS_VER}/modules/asyn${ASYN_VER}" > configure/RELEASE.local\
    && echo "STREAM=${WORK_DIR}/base-${EPICS_VER}/modules/StreamDevice" >> configure/RELEASE.local \
    && echo "PSClient_DBD += asyn.dbd" >> PSClientApp/src/Makefile \
    && echo "PSClient_DBD += stream.dbd" >> PSClientApp/src/Makefile \
    && echo "PSClient_DBD += drvAsynIPPort.dbd" >> PSClientApp/src/Makefile \
    && echo "PSClient_LIBS += asyn" >> PSClientApp/src/Makefile \
    && echo "PSClient_LIBS += stream" >> PSClientApp/src/Makefile \
    && echo "DB += PSTest.db" >> PSClientApp/Db/Makefile \
    && sed -i -e "s|#dbLoadRecords(\"db/xxx.db\",\"user=dev_beamline_sys\")|dbLoadRecords(\"db/PS.db\")\nepicsEnvSet(\"STREAM_PROTOCOL_PATH\", \".:../../PSTestClientApp/Db\")\ndrvAsynIPPortConfigure(\"PS1\", \"192.168.0.2:50001\")|g" iocBoot/iocPSClient/st.cmd

# Copy db file from local
WORKDIR ${APP_DIR}/PSClientApp/Db
COPY /PS_DB/* .

# Make app
RUN cd ${APP_DIR} \
    && make \
    && chmod +x iocBoot/iocPSClient/st.cmd \
    && yum remove -y wget gcc-c++ git readline-devel perl-devel make

# Runner image
FROM busybox:1.33.0-glibc

# Copy files from builder image
WORKDIR /usr/local/epics
COPY --from=builder /usr/local/epics .
WORKDIR /
COPY --from=builder /pkgs/lib64/*.so.* /lib64/
WORKDIR /
COPY --from=builder /PSApp /PSApp

# Expose port
EXPOSE 5064-5065/tcp
EXPOSE 5064-5065/udp

# Execute command
ENTRYPOINT ["/PSApp/iocBoot/iocPSClient/st.cmd"]
# CMD specifies arguments of entrypoint.
# This variables can be overridden when this image is executed.
#CMD ["-d", "/db/test.db"]
