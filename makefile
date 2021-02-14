
CC=clang
CFLAGS=-lssl -lcrypto
DEBUG_FLAG=-g
BINS=server
OBJS=server.o

server: server.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

debug: server.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $^

clean:
	rm -r $(OBJS)
	rm -r $(BINS)

