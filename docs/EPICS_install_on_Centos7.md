# EPICS install on CentOS7

## _This instruction is based on following websites_ :

- http://cerldev.kek.jp/trac/EpicsUsersJP/wiki/epics/install/epics_base
- https://www.programmersought.com/article/30756036224/

## 1. Install necessary packages

As a first step, you should install necessary packages as follows.

```shell
$ sudo yum update
$ sudo yum groupinstall base "Development tools" --setopt=group_package_types=mandatory,default,optional
$ sudo yum install perl perl-devel readline-devel 

```

## 2. Install EPICS

Make the working directory and download the archive file.

```shell
$ sudo mkdir -p /usr/local/epics/arc && cd $_
$ sudo wget https://epics.anl.gov/download/base/base-3.15.8.tar.gz
```

Unzip the archive file and move the generated directory to parent directory.

```shell
$ sudo tar zxvf base-3.15.8.tar.gz
$ sudo mv base-3.15.8 ../ && cd ../base-3.15.8
```

Make the project.

```shell
$ sudo make
```

If there is no error, then edit ~/.zshrc (or maybe .bashrc) and add following commands.

```shell
EPICS_HOST_ARCH=linux-x86_64
export EPICS_HOST_ARCH
export PATH=$PATH:/usr/local/epics/base-3.15.8/bin/linux-x86_64/
EPICS_BASE=/usr/local/epics/base-3.15.8/
export EPICS_BASE
```

Finally, don't forget to reload the .zshrc as follows.

```shell
$ source ~/.zshrc
```

## 3. Open the port

Before testing EPICS, you should open the connection port of 5064/5065.

It can be easily done using _firewall-cmd_ command([ref](https://eng-entrance.com/linux-centos-firewall#i)).

First, create an config file for epics.

```shell
$ cd /usr/lib/firewalld/services
$ sudo vim epics.xml
```

Contents of epics.xml should be as follows:

```xml
<?xml version="1.0" encoding="utf-8"?>
<service>
  <short>EPICS</short>
  <description>EPICS Control System port</description>
  <port protocol="tcp" port="5064"/>
  <port protocol="tcp" port="5065"/>
  <port protocol="udp" port="5064"/>
  <port protocol="udp" port="5065"/>
</service>
```

Don't forget to reload firewall-cmd.

```shell
$ firewall-cmd --reload
```

Then, add the service to firewall-cmd.

```shell
$ sudo firewall-cmd --zone=public --add-service=epics
```

## 4. Create First IOC instance

Here, we create the test IOC instance. First, make the working directory.

```shell
$ mkdir -p ~/Epics_test/MyFirstIoc && cd $_
```

Create an app named _''MyFirstIoc''_ using _''example''_ template.

```shell
$ makeBaseApp.pl -t example MyFirstIoc
```

Create an iocBoot directory.

```shell
$ makeBaseApp.pl -i -t example MyFirstIoc
```

If you are asked an application name, type _MyFirstIoc_.

Next, make the project.

```shell
$ make
```

If you got no error, then add permission to st.cmd to execute.

```shell
$ cd iocBoot/iocMyFirstIoc
$ chmod +x st.cmd
```

Then, run st.cmd.

```shell
$ ./st.cmd
```

If you successfully boot the instance, you got a message like this:

```shell
#!../../bin/linux-x86_64/MyFirstIoc
## You may have to change MyFirstIoc to something else
## everywhere it appears in this file
< envPaths
epicsEnvSet("IOC","iocMyFirstIoc")
epicsEnvSet("TOP","/home/dev_beamline_sys/Epics_tests/MyFirstIoc")
epicsEnvSet("EPICS_BASE","/usr/local/epics/base-3.15.8")
cd "/home/dev_beamline_sys/Epics_tests/MyFirstIoc"
## Register all support components
dbLoadDatabase "dbd/MyFirstIoc.dbd"
MyFirstIoc_registerRecordDeviceDriver pdbbase
## Load record instances
dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/dbSubExample.db", "user=dev_beamline_sys"
## Set this to see messages from mySub
#var mySubDebug 1
## Run this to trace the stages of iocInit
#traceIocInit
cd "/home/dev_beamline_sys/Epics_tests/MyFirstIoc/iocBoot/iocMyFirstIoc"
iocInit
Starting iocInit
############################################################################
## EPICS R3.15.8
## EPICS Base built Apr 14 2021
############################################################################
iocRun: All initialization complete
## Start any sequence programs
#seq sncExample, "user=dev_beamline_sys"
```

You can see database list by the following command.

```shell
epics> dbl
```

At the end, let's monitor the contents of database.

Open new window of the terminal, and type following command.

```shell
$ camonitor dev_beamline_sys:ai1
```

If you successfully connected, you got messages like this.

```shell
dev_beamline_sys:ai1           2021-04-14 23:13:07.326747 6 HIGH MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:08.326770 7 HIGH MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:09.326748 8 HIHI MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:10.326767 9 HIHI MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:11.326745 0 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:12.326777 1 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:13.326749 2 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:14.326768 3 LOW MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:15.326766 4 LOW MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:16.326755 5  
dev_beamline_sys:ai1           2021-04-14 23:13:17.326768 6 HIGH MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:18.326783 7 HIGH MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:19.326753 8 HIHI MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:20.326775 9 HIHI MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:21.326744 0 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:22.326770 1 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:23.326755 2 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:24.326798 3 LOW MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:25.326740 4 LOW MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:26.326781 5  
dev_beamline_sys:ai1           2021-04-14 23:13:27.326795 6 HIGH MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:28.326779 7 HIGH MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:29.326766 8 HIHI MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:30.326783 9 HIHI MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:31.326767 0 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:32.326785 1 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:33.326737 2 LOLO MAJOR
dev_beamline_sys:ai1           2021-04-14 23:13:34.326814 3 LOW MINOR
dev_beamline_sys:ai1           2021-04-14 23:13:35.326713 4 LOW MINOR
```

