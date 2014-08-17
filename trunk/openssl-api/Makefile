CC=gcc
CFLAGS=-I. -g
LIBS = -lpthread -lssl
DEPS = ssl.h utils.h
CLIENT_OBJ = client.o
MEM_OBJ = mem.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: client mem

client: $(CLIENT_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

mem: $(MEM_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f client mem *.o
