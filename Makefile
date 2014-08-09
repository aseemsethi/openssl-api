CC=gcc
CFLAGS=-I. -g
LIBS = -lpthread -lssl
DEPS = ssl.h utils.h
OBJ = client.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f client *.o
