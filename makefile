
CC=clang
CFLAGS=-g -lssl -lcrypto
BINS=server client
OBJS=server.o client.o

all: server client

server: server.o
	$(CC) $(CFLAGS) -o $@ $^

client: client.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r $(OBJS)
	rm -r $(BINS)

