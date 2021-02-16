
CC=clang
CFLAGS=-g
LIBS=-lssl -lcrypto -luriparser
BINS=server
OBJS=server.o uri.c

server: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r $(OBJS)
	rm -r $(BINS)

