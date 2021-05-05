# Connecting test to PowerSupply via epics

[Back to main page](../index.md)

## 1. Craete IOC

### Make app from template

```shell
$ mkdir PSTestApp && cd $_
$ makeBaseApp.pl -t ioc PSTestClient
$ makeBaseApp.pl -i -t ioc PSTestClient
```

### Edit RELEASE file

```shell
$ vim configure/RELEASE
```

Add ASYN, STREAM path

``` shell
# RELEASE - Location of external support modules
#
# IF YOU MAKE ANY CHANGES to this file you must subsequently
# do a "gnumake rebuild" in this application's top level
# directory.
#
# The build process does not check dependencies against files
# that are outside this application, thus you should do a
# "gnumake rebuild" in the top level directory after EPICS_BASE
# or any other external module pointed to below is rebuilt.
#
# Host- or target-specific settings can be given in files named
#  RELEASE.$(EPICS_HOST_ARCH).Common
#  RELEASE.Common.$(T_A)
#  RELEASE.$(EPICS_HOST_ARCH).$(T_A)
#
# This file is parsed by both GNUmake and an EPICS Perl script,
# so it can ONLY contain definititions of paths to other support
# modules, variable definitions that are used in module paths,
# and include statements that pull in other RELEASE files.
# Variables may be used before their values have been set.
# Build variables that are NOT used in paths should be set in
# the CONFIG_SITE file.

# Variables and paths to dependent modules:
#MODULES = /path/to/modules
#MYMODULE = $(MODULES)/my-module
ASYN=/usr/local/epics/base-3.15.8/modules/asyn4-38 # Add
STREAM=/usr/local/epics/base-3.15.8/modules/StreamDevice # Add

# If using the sequencer, point SNCSEQ at its top directory:
#SNCSEQ = $(MODULES)/seq-ver

# EPICS_BASE should appear last so earlier modules can override stuff:
EPICS_BASE = /usr/local/epics/base-3.15.8

# Set RULES here if you want to use build rules from somewhere
# other than EPICS_BASE:
#RULES = $(MODULES)/build-rules

# These allow developers to override the RELEASE variable settings
# without having to modify the configure/RELEASE file itself.
-include $(TOP)/../RELEASE.local
-include $(TOP)/configure/RELEASE.local
```

### Edit Makefile

```shell
$ vim PSTestClientApp/src/Makefile
```

Add PSTestClient_DBD, PSTestClient_LIBS

```shell
TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = PSTestClient
# PSTestClient.dbd will be created and installed
DBD += PSTestClient.dbd

# PSTestClient.dbd will be made up from these files:
PSTestClient_DBD += base.dbd

# Include dbd files from all support applications:
#PSTestClient_DBD += xxx.dbd
PSTestClient_DBD += asyn.dbd # Add
PSTestClient_DBD += stream.dbd # Add
PSTestClient_DBD += drvAsynIPPort.dbd # Add

# Add all the support libraries needed by this IOC
#PSTestClient_LIBS += xxx
PSTestClient_LIBS += asyn # Add
PSTestClient_LIBS += stream # Add

# PSTestClient_registerRecordDeviceDriver.cpp derives from PSTestClient.dbd
PSTestClient_SRCS += PSTestClient_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
PSTestClient_SRCS_DEFAULT += PSTestClientMain.cpp
PSTestClient_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#PSTestClient_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
PSTestClient_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
```

### Add db file

Add database, protocol file to PSTestClientApp/Db/

#### database

make PSTest.db

```sh
record(stringin, "test:getDOR")
{
  field(DESC, "get status from PS")
  field(DTYP, "stream")
  field(INP, "@tc-l2s.proto getdor PS1")
}
```

#### protocol

make tc-l2s.proto

```shell
Terminator = CR LF;
#
getDOR {
  out "@DOR?";
  in "%5c";
}
```

### Edit Makefile

``` shell
$ vim PSTestClientApp/Db/Makefile
```

Add DB

```shell
TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += PSTest.db # Add

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
```

### Edit st.cmd

``` shell
$ vim iocBoot/iocPSTestClient/st.cmd
```

Add record instance

```shell
#!../../bin/linux-x86_64/PSTestClient

## You may have to change PSTestClient to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/PSTestClient.dbd"
PSTestClient_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=dev_beamline_sys")
dbLoadRecords("db/PSTest.db") # Add
epicsEnvSet("STREAM_PROTOCOL_PATH", ".:../../PSTestClientApp/Db") # Add
drvAsynIPPortConfigure("PS1", "[IP]:[port]") # Add, please replace IP and port correctly

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=dev_beamline_sys"
```

### Make the project

``` shell
$ cd ~/PSTestApp
$ make
```

### Execute IOC

```shell
$ cd iocBoot/iocPSTestClient
$ chmod +x st.cmd
$ ./st.cmd

#!../../bin/linux-x86_64/PSTestClient
## You may have to change PSTestClient to something else
## everywhere it appears in this file
< envPaths
epicsEnvSet("IOC","iocPSTestClient")
epicsEnvSet("TOP","/home/dev_beamline_sys/PSTestApp")
epicsEnvSet("ASYN","/usr/local/epics/base-3.15.8/modules/asyn4-38")
epicsEnvSet("STREAM","/usr/local/epics/base-3.15.8/modules/StreamDevice")
epicsEnvSet("EPICS_BASE","/usr/local/epics/base-3.15.8")
cd "/home/dev_beamline_sys/PSTestApp"
## Register all support components
dbLoadDatabase "dbd/PSTestClient.dbd"
PSTestClient_registerRecordDeviceDriver pdbbase
## Load record instances
#dbLoadRecords("db/xxx.db","user=dev_beamline_sys")
dbLoadRecords("db/PSTest.db")
epicsEnvSet("STREAM_PROTOCOL_PATH", ".:../../PSTestClientApp/Db")
drvAsynIPPortConfigure("PS1", "[IP]:[port]")
cd "/home/dev_beamline_sys/PSTestApp/iocBoot/iocPSTestClient"
iocInit
Starting iocInit
############################################################################
## EPICS R3.15.8
## EPICS Base built Apr 14 2021
############################################################################
iocRun: All initialization complete
## Start any sequence programs
#seq sncxxx,"user=dev_beamline_sys"
epics>
```

You can check your db by 'dbl' command:

```shell
epics> dbl
test:getDOR
```

Next, execute the record 'test:getDOR'

```shell
epics> dbpf test:getDOR  PROC 1
DBR_STRING:          "PROC"   
2021/05/05 21:56:40.713125 PS1 test:getDOR: Timeout after reading 5 bytes "@DO00"
```

Maybe the error is not so critical

Let's open another terminal window and execute camonitor

```shell
$ camonitor test:getDOR
test:getDOR                    2021-05-05 21:56:40.713456 @DO00 READ INVALID
```

We got the string '@DO00' from Power Supply.