
CC=clang
CFLAGS=-g -Wall
LIBS=-lssl -lcrypto -luriparser
BINS=server
OBJS=server.o uri.o file.o response.o

server: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -r $(OBJS)
	rm -r $(BINS)

