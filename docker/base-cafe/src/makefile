#CAFE version to link to
CAFE_VERSION=cafe-noqt-1.12.5-gcc-6.3.0

EPICS_BASE=/usr/local/epics/base-3.15.8

#CAFE project base
CAFE_BASE=/opt/gfa/cafe
CAFE_CPP_BASE=${CAFE_BASE}/cpp
BOOST_BASE=/usr/include/boost

INCLUDE_PATH += -I$(CAFE_CPP_BASE)/$(CAFE_VERSION)/include \
 -I$(EPICS_BASE)/include -I$(EPICS_BASE)/include/os/Linux \
 -I$(BOOST_BASE) 
 
LIB_DIR += -L$(EPICS_BASE)/lib/${EPICS_HOST_ARCH} \
 -Wl,-rpath,$(EPICS_BASE)/lib/${EPICS_HOST_ARCH} \
 -L$(CAFE_CPP_BASE)/$(CAFE_VERSION)/lib \
 -Wl,-rpath,$(CAFE_CPP_BASE)/$(CAFE_VERSION)/lib

LIBS += -lcafe -lca -lCom 
 

cafeTest: cafeTest.cc callbacks.h
	g++ cafeTest.cc -o cafeTest \
 	$(INCLUDE_PATH) $(LIB_DIR) $(LIBS)
	

clean:
	rm -f cafeTest.o cafeTest

