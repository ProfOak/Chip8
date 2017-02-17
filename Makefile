CC=gcc
SOURCES=chip8.c chip8.h graphics.c graphics.h main.c

ifeq ($(OS),Windows_NT)
	EXECUTABLE=chip8.exe
else
	EXECUTABLE=chip8
endif

CFLAGS=-o $(EXECUTABLE) -lSDL2 -std=c11 -Wall
all: build

build: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

debug: $(SOURCES)
	$(CC) -g $(CFLAGS) $(SOURCES)

clean: $(EXECUTABLE)
	rm -f $(EXECUTABLE)
