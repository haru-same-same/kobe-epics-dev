#Inspired by pklaus/docker-archiver-appliance

# Image for download and extrct archiver appliance
FROM alpine AS extractor

# Maintainer
LABEL maintainer="abes <abes@stu.kobe-u.ac.jp>"

# Setting variables
ENV RELEASE=v0.0.1_SNAPSHOT_13-Nov-2019 \
    RELEASE_FILE=archappl_v0.0.1_SNAPSHOT_13-November-2019T15-45-42.tar.gz

# Update and install necessary packages
RUN apk update && apk add \
    tar \
    unzip \
    curl

# Download and extract archiver appliance
WORKDIR /archiver_appliance
RUN curl -OL https://github.com/slacmshankar/epicsarchiverap/releases/download/${RELEASE}/${RELEASE_FILE} \
    && tar -xf ${RELEASE_FILE} \
    && rm ${RELEASE_FILE}

# Runner image
FROM tomcat:9.0.45-jdk16-openjdk-slim-buster AS runner

SHELL ["/bin/bash", "-c"]

# Copy license files
COPY --from=extractor /archiver_appliance/NOTICE \
                      /archiver_appliance/LICENSE \
                      /archiver_appliance/Apache_2.0_License.txt \
                      /archiver_appliance/RELEASE_NOTES /

# Copy war files
COPY --from=extractor /archiver_appliance/*.war /usr/local/tomcat/webapps/

# Copy archapel file
COPY /etc/archappl/* /etc/archappl/

# Create directories we store data and linking 
RUN mkdir -p /storage/{sts, mts, lts, logs} \
    && ln -s /etc/archappl/log4j.properties /usr/local/tomcat/lib/log4j.properties \
    && mv /usr/local/tomcat/conf/server.xml{,.dist} \
    && ln -s /etc/archappl/tomcat_conf_server.xml /usr/local/tomcat/conf/server.xml \
    && rmdir /usr/local/tomcat/logs \
    && ln -s /storage/logs /usr/local/tomcat/logs

# Be generous with the heap
ENV JAVA_OPTS="-XX:+UseG1GC -Xms4G -Xmx4G -ea"
# Tell the appliance that we are deploying all the components in one VM;
# this reduces the thread count and other parameters in an effort to optimize memory:
#ENV ARCHAPPL_ALL_APPS_ON_ONE_JVM=true
ENV ARCHAPPL_APPLIANCES=/etc/archappl/appliances.xml
ENV ARCHAPPL_MYIDENTITY=appliance0
ENV ARCHAPPL_POLICIES=/etc/archappl/policies.py
ENV ARCHAPPL_PROPERTIES_FILENAME=/etc/archappl/archappl.properties
ENV ARCHAPPL_PERSISTENCE_LAYER=org.epics.archiverappliance.config.persistence.InMemoryPersistence
ENV ARCHAPPL_SHORT_TERM_FOLDER=/storage/sts
ENV ARCHAPPL_MEDIUM_TERM_FOLDER=/storage/mts
ENV ARCHAPPL_LONG_TERM_FOLDER=/storage/lts
ENV EPICS_CA_AUTO_ADDR_LIST=yes
ENV EPICS_CA_ADDR_LIST=

EXPOSE 17665

CMD ["catalina.sh", "run"]
