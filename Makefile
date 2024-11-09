CC = gcc 

all: build

build: src/*.c src/*.h
	$(CC) src/*.c -o kite -g

install: src/*.c src/*.h
	$(CC) src/*.c -o /usr/bin/kite -g
