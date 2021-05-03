# Builder image
FROM centos:7.9.2009 AS builder

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV WORK_DIR=/usr/local/epics

ENV EPICS_VER=3.15.8 \
    USER=epics-dk \
    EPICS_BASE=${WORK_DIR}/base-${EPICS_VER}
    EPICS_HOST_ARCH=linux-x86_64 \
    PATH=${PATH}:${EPICS_BASE}/bin/${EPICS_HOST_ARCH}

# Update yum and install necessary packages
RUN yum update -y && yum install -y \
    wget \
    gcc-c++ \
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
    && cp --parents /lib64/libtinfo.so.5 /pkgs \

# Download and install epics
RUN wget -q https://epics.anl.gov/download/base/base-$EPICS_VER.tar.gz \
    && tar -zxvf base-${EPICS_VER}.tar.gz \
    && mkdir ${WORK_DIR} \
    && mv base-${EPICS_VER} ${WORK_DIR} \
    && rm base-${EPICS_VER}.tar.gz \
    && cd ${EPICS_BASE} \
    && make \
    && yum remove -y wget gcc-c++ readline-devel perl-devel make

# Runner image
FROM busybox:1.33.0-glibc

# Setting variables
ENV EPICS_VER=3.15.8 \
    EPICS_HOST_ARCH=linux-x86_64

ENV PATH=${PATH}:/usr/local/epics/base-${EPICS_VER}/bin/${EPICS_HOST_ARCH}

# Copy files from builder image
WORKDIR /usr/local/epics
COPY --from=builder /usr/local/epics .
WORKDIR /
COPY --from=builder /pkgs/lib64/*.so.* /lib64/

# Expose port
EXPOSE 5064-5065/tcp
EXPOSE 5064-5065/udp

# Execute command
ENTRYPOINT ["softIoc"]
# CMD specifies arguments of entrypoint.
# This variables can be overridden when this image is executed.
CMD ["-d", "/db/test.db"]
