CC=gcc
SOURCES=main.c chip8.c chip8.h
EXECUTABLE=chip8.exe
FLAGS=-o $(EXECUTABLE)

all: build

build: $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES)

clean: $(EXECUTABLE)
	rm $(EXECUTABLE)
