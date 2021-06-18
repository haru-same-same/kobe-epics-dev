# Cafe Install

※ for abes0/epics-base

## Install dependencies

``` bash
$ apt install automake libtool libboost-dev
```

## Download & Install Cafe

### Download & unpack

``` bash
$ cd /home/
$ wget https://gitlab.psi.ch/cafe/cpp/-/archive/cafe-1.12.5/cpp-cafe-1.12.5.tar.gz
$ tar zxvf cpp-cafe-1.12.5.tar.gz
$ cd cpp-cafe-1.12.5
```

### Modify package files

```bash
# autogen_rel_noqt.sh

rm -f configure.ac
ln -s configureCafe.ac configure.ac

aclocal  --force -I m4
libtoolize --force --copy
autoconf --force
autoheader --force
automake --force --add-missing --copy
autoreconf

#Check what the EPICS RELEASE IS from ${EPICS}/base
#Used by ./configure
#Assumes format ${EPICS}/base-3.14.12

EPICS=/usr/local/epics # add
EB=$(readlink ${EPICS}/base) # -> EB=base-3.15.8
echo 'EPICS BASE IS=' $EB
EB1=$(echo $EB | sed -e "s/[a-zA-Z]*-//g")
EBMAJ=$(echo $EB1 | sed -e "s/[[:punct:]].*//g")
EBMIN1=$(echo $EB1 | sed -e "s/^[[:digit:]]*[[:punct:]]//g")
EBMIN=$(echo $EBMIN1 | sed -e "s/[[:punct:]][[:digit:]]*//g")
EBPAT=$(echo $EB1 | sed -e "s/[[:digit:]]*[[:punct:]]//g")
echo EPICS_MAJOR=$EBMAJ 
echo EPICS_MINOR=$EBMIN 
echo EPICS_PATCH=$EBPAT

export CAFE_EPICS_V_MAJOR=$EBMAJ
export CAFE_EPICS_V_MINOR=$EBMIN
export CAFE_EPICS_V_PATCH=$EBPAT


CAFE_VERSION=cafe-noqt-1.12.5-gcc-6.3.0


./configure \
    --prefix=/opt/gfa/cafe/cpp/${CAFE_VERSION} \
    --libdir=/opt/gfa/cafe/cpp/${CAFE_VERSION}/lib \
    --enable-boost-inc --with-boost-inc=/opt/gfa/cafe/boost/boost_1_61_0/include \ # -> --enable-boost-inc --with-boost-inc=/usr/include/boost/
    --enable-epics --with-epics=${EPICS}/base # -> --enable-epics --with-epics=${EPICS}/base-3.15.8

	
   
unset  CAFE_EPICS_V_MAJOR
unset  CAFE_EPICS_V_MINOR
unset  CAFE_EPICS_V_PATCH

#cp include/config.h include/os/Linux/cafe-noqt/config.h # comment out this line
```

### Modify epics include file

``` bash
$ vim /usr/local/epics/base-3.15.8/include/compilerDependencies.h

# compilerDependencies.h

/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/*
 * Author: 
 * Jeffrey O. Hill
 * johill@lanl.gov
 */

#ifndef compilerDependencies_h
#define compilerDependencies_h

#include "compilerSpecific.h" # -> #include "compiler/gcc/compilerSpecific.h"

#ifdef __cplusplus
```

### Run autogen

```bash
$ mkdir m4
$ source autogen_rel_noqt.sh
$ make
$ make install
$ export LD_LIBRARY_PATH=/opt/gfa/cafe/cpp/cafe-noqt-1.12.5-gcc-6.3.0/lib
```

