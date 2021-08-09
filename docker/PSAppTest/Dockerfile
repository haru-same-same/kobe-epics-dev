# Source image
FROM abes0/epics-stream:latest

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

WORKDIR /

RUN apt update -y && apt upgrade -y && apt install -y python3

# Create IOC App
RUN mkdir ${APP_DIR} && cd ${APP_DIR} \
    && ${WORK_DIR}/base-${EPICS_VER}/bin/${EPICS_HOST_ARCH}/makeBaseApp.pl -t ioc PSClient \
    && ${WORK_DIR}/base-${EPICS_VER}/bin/${EPICS_HOST_ARCH}/makeBaseApp.pl -i -t ioc PSClient \
    && echo

# Copy db file from local
WORKDIR ${APP_DIR}/PSClientApp/Db
COPY /PS_DB/* .

# Modify setting files using python script (To be implemented)
# Copy python script from local
WORKDIR /
COPY modFile.py .

#RUN python modFile.py
RUN python3 modFile.py ${WORK_DIR} ${EPICS_VER} ${ASYN_VER} ${USER}

# Make app
WORKDIR ${APP_DIR}
RUN make \
    && chmod +x iocBoot/iocPSClient/st.cmd
