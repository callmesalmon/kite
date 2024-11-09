CC=gcc 
CFLAGS=-std=c99 -g

BIN=/usr/bin/kite
SOURCE=src/*.c
HEADERS=src/*.h

$(BIN): $(HEADERS) $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(BIN)

.PHONY clean:
	rm -f $(BIN)
