CC = gcc 

all: build

build: src/*.c src/*.h
	$(CC) *.c -o kite -g

install: src/*.c src/*.h
	$(CC) *.c -o /usr/bin/kite -g
