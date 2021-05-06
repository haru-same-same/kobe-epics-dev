# Builder image
FROM abes0/kobe-bl-epics:stream-latest AS builder

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV EPICS_VER=3.15.8 \
    ASYN_VER=4-38 \
    WORK_DIR=/usr/local/epics \
    MOD_DIR=${EPICS_BASE}/modules \
    STREAM_DIR=${MOD_DIR}/StreamDevice \
    APP_DIR=/PSApp

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

# Copy Packages in order to use them in busybox
RUN mkdir /pkgs \
    && cp --parents /lib64/libstdc++.so.6 /pkgs \
    && cp --parents /lib64/libm.so.6 /pkgs \
    && cp --parents /lib64/libgcc_s.so.1 /pkgs \
    && cp --parents /lib64/libreadline.so.6 /pkgs \
    && cp --parents /lib64/librt.so.1 /pkgs \
    && cp --parents /lib64/libdl.so.2 /pkgs \
    && cp --parents /lib64/libtinfo.so.5 /pkgs

# Copy db file from local
WORKDIR ${APP_DIR}/PSClientApp/Db
COPY /PS_DB/* .

# Make app
RUN cd ${APP_DIR} \
    && make \
    && chmod +x iocBoot/iocPSClient/st.cmd \
    && yum remove -y wget gcc-c++ git readline-devel perl-devel make

# Runner image
FROM busybox:1.33.0-glibc AS runner

# Copy files from builder image
WORKDIR /usr/local/epics
COPY --from=builder /usr/local/epics .
WORKDIR /
COPY --from=builder /pkgs/lib64/*.so.* /lib64/
WORKDIR /
COPY --from=builder /PSApp .

# Expose port
EXPOSE 5064-5065/tcp
EXPOSE 5064-5065/udp

# Execute command
WORKDIR ${APP_DIR}/iocBoot/iocPSClient
ENTRYPOINT ["/bin/sh"]
#CMD ["st.cmd"]
