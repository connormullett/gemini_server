
CC=clang
CFLAGS=-g
LIBS=-lssl -lcrypto
BINS=server
OBJS=server.o

server: server.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r $(OBJS)
	rm -r $(BINS)

