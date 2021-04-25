HEADERS = counters.h resource.h settings.h singleinstance.h snapwindow.h
OBJS =	release/cpumeter.o release/resource.o
CC = g++
RC = windres
LDFLAGS = -s -mwindows -fexceptions -lgdi32 -lpdh -static -lstdc++
CFLAGS= -O3

all: release/cpumeter.exe

release/cpumeter.exe: ${OBJS}
	${CC} -o "$@" ${OBJS} ${LDFLAGS}
	
release/%.o: %.cpp ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

release/resource.o: cpumeter.rc utilities-system-monitor.ico resource.h
	${RC} -i $< -o $@
