#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-xilinx-linux-gnueabi-gcc
CCC=arm-xilinx-linux-gnueabi-g++
CXX=arm-xilinx-linux-gnueabi-g++
FC=gfortran
AS=arm-xilinx-linux-gnueabi-as

# Macros
CND_PLATFORM=zynq7000-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/IPComm/TcpComm.o \
	${OBJECTDIR}/IPComm/udpComm.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/pcmsocket/pcmsocket.o \
	${OBJECTDIR}/websocket/websocket.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=websocket/lib/libwebsockets.so websocket/lib/libssl.so websocket/lib/libcrypto.so

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver: websocket/lib/libwebsockets.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver: websocket/lib/libssl.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver: websocket/lib/libcrypto.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver ${OBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${OBJECTDIR}/IPComm/TcpComm.o: IPComm/TcpComm.cpp
	${MKDIR} -p ${OBJECTDIR}/IPComm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iwebsocket/include -I/home/zwenqian/Tools/openssl/openssl-1.1.1c/arm-linux/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IPComm/TcpComm.o IPComm/TcpComm.cpp

${OBJECTDIR}/IPComm/udpComm.o: IPComm/udpComm.cpp
	${MKDIR} -p ${OBJECTDIR}/IPComm
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iwebsocket/include -I/home/zwenqian/Tools/openssl/openssl-1.1.1c/arm-linux/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/IPComm/udpComm.o IPComm/udpComm.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iwebsocket/include -I/home/zwenqian/Tools/openssl/openssl-1.1.1c/arm-linux/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/pcmsocket/pcmsocket.o: pcmsocket/pcmsocket.cpp
	${MKDIR} -p ${OBJECTDIR}/pcmsocket
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iwebsocket/include -I/home/zwenqian/Tools/openssl/openssl-1.1.1c/arm-linux/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/pcmsocket/pcmsocket.o pcmsocket/pcmsocket.cpp

${OBJECTDIR}/websocket/websocket.o: websocket/websocket.cpp
	${MKDIR} -p ${OBJECTDIR}/websocket
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iwebsocket/include -I/home/zwenqian/Tools/openssl/openssl-1.1.1c/arm-linux/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/websocket/websocket.o websocket/websocket.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libwebsockets.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libssl.so ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcrypto.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/websocketserver

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
