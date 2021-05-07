# Builder image
FROM abes0/kobe-bl-epics:stream-latest AS builder

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV EPICS_VER=3.15.8 \
    ASYN_VER=4-38 \
    EPICS_HOST_ARCH=linux-x86_64 \
    WORK_DIR=/usr/local/epics \
    MOD_DIR=${EPICS_BASE}/modules \
    STREAM_DIR=${MOD_DIR}/StreamDevice \
    APP_DIR=/PSApp \
    USER=PSAppTest

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
    && sed -i -e "s|#DB += xxx.db|DB += PS.db|g" PSClientApp/Db/Makefile \
    && sed -i -e "s|#dbLoadRecords(\"db/xxx.db\",\"user=${USER}\")|dbLoadRecords(\"db/PS.db\")\nepicsEnvSet(\"STREAM_PROTOCOL_PATH\", \".:../../PSClientApp/Db\")\ndrvAsynIPPortConfigure(\"PS1\", \"10.37.0.196:50001\")|g" iocBoot/iocPSClient/st.cmd

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

# Modify setting files using python script (To be implemented)
# Copy python script from local
#WORKDIR /
#COPY modFile.py .

#RUN python modFile.py

# Make app
WORKDIR ${APP_DIR}
RUN make \
    && chmod +x iocBoot/iocPSClient/st.cmd

# Runner image
FROM busybox:1.33.0-glibc AS runner

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
WORKDIR PSApp/iocBoot/iocPSClient
ENTRYPOINT ["/bin/sh"]
CMD ["-c", "./st.cmd"]
