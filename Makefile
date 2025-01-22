CC     = clang
CFLAGS = -std=c99 -g

BINARY = /usr/local/bin/kite
SOURCE = src/*.c
HEADER = src/*.h

all: $(BINARY)

$(BINARY): $(HEADER) $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(BINARY)
