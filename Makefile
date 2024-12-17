CC=gcc 
CFLAGS=-std=c99 -g

BIN=/usr/local/bin/kite
SOURCE=src/*.c
HEADERS=src/*.h

all: $(BIN)

$(BIN): $(HEADERS) $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(BIN)
