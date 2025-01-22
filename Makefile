.PHONY: build install

CC     = clang
CFLAGS = -std=c99 -g

BINARY        = kite
BINARY_PREFIX = /usr/local/bin

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

all: build install

build: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARY)

install: build
	mv $(BINARY) $(BINARY_PREFIX)/$(BINARY)
