CC=gcc
SOURCES=main.c chip8.c chip8.h

ifeq ($(OS),Windows_NT)
	EXECUTABLE=chip8.exe
else
	EXECUTABLE=chip8
endif

CFLAGS=-o $(EXECUTABLE)
all: build

build: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

debug: $(SOURCES)
	$(CC) -g $(CFLAGS) $(SOURCES)

clean: $(EXECUTABLE)
	rm -f $(EXECUTABLE)
