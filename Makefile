CCP=g++
CC=gcc
RM=rm -f

CPPFLAGS=-g -c -Wall -std=c++11
ifeq ($(INI_STOP_ON_FIRST_ERROR), 1)
CFLAGS=-g -c -Wall -DINI_STOP_ON_FIRST_ERROR
else
CFLAGS=-g -c -Wall
endif

LDFLAGS=
CPP_SOURCES=$(wildcard *.cpp)
CPP_OBJECTS=$(CPP_SOURCES:.cpp=.o)
C_SOURCES = $(wildcard *.c)
C_SOURCES += $(wildcard src/*.c)
C_OBJECTS=$(C_SOURCES:.c=.o)
INCDIR = $(CURDIR)/include


EXECUTABLE=mpu6050

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(C_OBJECTS) $(CPP_OBJECTS)
		$(CCP) $(LDFLAGS) $(C_OBJECTS)  $(CPP_OBJECTS) -o $@
clean:
	$(RM) *.o
	$(RM) $(EXECUTABLE)
.cpp.o:
		$(CCP) $(CPPFLAGS) $< -o $@
.c.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -o $@


