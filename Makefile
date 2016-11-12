CC=gcc
CFLAGS=-shared -Wall -Werror -fPIC -O3 -std=gnu99 -I/usr/local/include/
LDFLAGS=-L/usr/lib/ -Wl,--whole-archive -lev3dev-c -Wl,--no-whole-archive

all:
	$(CC) $(CFLAGS) -o build/libbrick.so src/libbrick.c $(LDFLAGS)
