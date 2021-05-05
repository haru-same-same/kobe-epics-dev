# Install asyn driver and StreamDevice

[Back to main page](../index.md)

## asyn driver

### 1. Download and extract the module

```shell
$ cd /usr/local/epics/base-3.15.8
$ sudo mkdir modules && cd $_
$ sudo wget https://epics.anl.gov/download/modules/asyn4-38.tar.gz
$ sudo tar -zxvf asyn4-38.tar.gz
$ sudo rm asyn4-38.tar.gz
```

### 2. Edit configure file

```shell
$ sudo vim asyn4-38/configure/RELEASE
```

Comment out SNCSEQ setting and edit SUPPORT, EPICS_BASE path as follows:

``` shell
#RELEASE Location of external products

SUPPORT=/usr/local/epics/base-3.15.8/modules # Edit

#  IPAC is only necessary if support for Greensprings IP488 is required
#  IPAC release V2-7 or later is required.
IPAC=$(SUPPORT)/ipac-2-15

# SEQ is required for testIPServer
#SNCSEQ=$(SUPPORT)/seq-2-2-5 # Edit

#  EPICS_BASE 3.14.6 or later is required
EPICS_BASE=/usr/local/epics/base-3.15.8 # Edit

-include $(TOP)/../RELEASE.local
-include $(TOP)/../RELEASE.$(EPICS_HOST_ARCH).local
-include $(TOP)/configure/RELEASE.local
```

### 3. Make the module

```shell
$ cd asyn4-38
$ sudo make
```

## StreamDevice

### 1. Download the module

``` shell
$ cd /usr/local/epics/base-3.15.8/modules
$ sudo git clone https://github.com/paulscherrerinstitute/StreamDevice.git
```

### 2. Edit configure file

```shell
$ sudo vim StreamDevice/configure/RELEASE
```

Comment out CALC, PCRE setting and edit ASYN, EPICS_BASE path as follows:

```shell
#RELEASE Location of external products
# Run "gnumake clean uninstall install" in the application
# top directory each time this file is changed.
#
# NOTE: The build does not check dependencies on files
# external to this application. Thus you should run
# "gnumake clean uninstall install" in the top directory
# each time EPICS_BASE, SNCSEQ, or any other external
# module defined in the RELEASE file is rebuilt.

TEMPLATE_TOP=$(EPICS_BASE)/templates/makeBaseApp/top

# If you don't want to install into $(TOP) then
# define INSTALL_LOCATION_APP here
#INSTALL_LOCATION_APP=<fullpathname>

SUPPORT=$(TOP)/..
-include $(TOP)/../configure/SUPPORT.$(EPICS_HOST_ARCH)

ASYN=/usr/local/epics/base-3.15.8/modules/asyn4-38 # Edit
#CALC=$(SUPPORT)/calc-3-7 # Edit
#PCRE=$(SUPPORT)/pcre-7-2 # Edit

# EPICS_BASE usually appears last so other apps can override stuff:
EPICS_BASE=/usr/local/epics/base-3.15.8 # Edit

# These lines allow developers to override these RELEASE settings
# without having to modify this file directly.
-include $(TOP)/../RELEASE.local
-include $(TOP)/../RELEASE.$(EPICS_HOST_ARCH).local
-include $(TOP)/configure/RELEASE.local
```

### 3. Make the module

```shell
$ cd StreamDevice
$ sudo make
```



