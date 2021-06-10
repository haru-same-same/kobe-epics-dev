# Build from epics base image
FROM abes0/epics-base:latest

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV WORK_DIR=/usr/local/epics \
    EPICS_VER=3.15.8 \
    GATEWAY_VER=2.1.2

# Install ca gateway
RUN cd ${WORK_DIR}/base-${EPICS_VER} \
    && mkdir extensions && cd extensions \
    && wget https://github.com/epics-extensions/ca-gateway/archive/refs/tags/v${GATEWAY_VER}.tar.gz -O ca-gateway-${GATEWAY_VER}.tar.gz\
    && tar -zxvf ca-gateway-${GATEWAY_VER}.tar.gz \
    && rm ca-gateway-${GATEWAY_VER}.tar.gz \
    && cd ca-gateway-${GATEWAY_VER} \
    && echo "EPICS_BASE=${WORK_DIR}/base-${EPICS_VER}" > configure/RELEASE.local \
    && make

# PATH
ENV PATH $PATH:/usr/local/epics/base-3.15.8/extensions/ca-gateway-2.1.2/bin/linux-x86_64

CMD ["/bin/bash"]
