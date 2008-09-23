###############################################
# Macros
###############################################

CC = gcc
# CC = gccmemstat

CFLAGS = -Wall -ansi -pedantic `sdl-config --cflags`
CFLAGSLINK = -Wall -ansi -pedantic `sdl-config --cflags --libs`

###############################################
# Pattern rules
###############################################

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

###############################################
# Build rules for non-file targets
###############################################

all: game

clobber: clean
	rm -f *~ \#*\# core

clean:
	rm -f *.o game

###############################################
# Build rules for file targets
###############################################


game: main.c draw.o engine.o particlesys.o rect.o ship.o timer.o vector.o
	$(CC) $(CFLAGSLINK) -o game main.c draw.o engine.o particlesys.o \
	rect.o ship.o timer.o vector.o

draw.o: draw.h

engine.o: engine.h

particlesys.o: particlesys.h

rect.o: rect.h

ship.o: ship.h

timer.o: timer.h

vector.o: vector.h

