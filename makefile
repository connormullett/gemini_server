
CC=clang
CFLAGS=-g -Wall
OPTIMIZE_FLAGS=-o3
LIBS=-lssl -lcrypto -luriparser -lconfig
OBJS=server.o uri.o file.o response.o

server: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

build: $(OBJS)
	$(CC) $(OPTIMIZE_FLAGS) $(LIBS) -o server $^

clean:
	rm -r $(OBJS)
	rm -r server

