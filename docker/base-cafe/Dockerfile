# Build from epics base image
FROM abes0/epics-base:latest

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV WORK_DIR=/usr/local/epics \
    EPICS_VER=3.15.8

WORKDIR /home/

# Install Cafe
RUN apt install -y automake libtool libboost-dev \
    && wget https://gitlab.psi.ch/cafe/cpp/-/archive/cafe-1.12.5/cpp-cafe-1.12.5.tar.gz \
    && tar -zxvf cpp-cafe-1.12.5.tar.gz \
    && cd cpp-cafe-1.12.5 \
    && sed -i 's|EB=\$\(readlink \${EPICS}/base\)|EB=base-\${EPICS_VER}|g' autogen_rel_noqt.sh \
    && sed -i 's|/opt/gfa/cafe/boost/boost_1_61_0/include|/usr/include/boost/|g' autogen_rel_noqt.sh \
    && sed -i 's|--with-epics=\${EPICS}/base|--with-epics=\${EPICS}/base-3.15.8|g' autogen_rel_noqt.sh \
    && sed -i 's|cp include/config.h include/os/Linux/cafe-noqt/config.h|#cp include/config.h include/os/Linux/cafe-noqt/config.h|g' autogen_rel_noqt.sh \
    && sed -i 's|#include "compilerSpecific.h"|#include "compiler/gcc/compilerSpecific.h"|g' /usr/local/epics/base-3.15.8/include/compilerDependencies.h \
    && mkdir m4 \
    && . ./autogen_rel_noqt.sh \
    && make \
    && make install \
    && cd .. \
    && rm -rf cpp-cafe-1.12.5.tar.gz cpp-cafe-1.12.5

# PATH
ENV LD_LIBRARY_PATH=/opt/gfa/cafe/cpp/cafe-noqt-1.12.5-gcc-6.3.0/lib

CMD ["/bin/bash"]
