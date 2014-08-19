CC=gcc
CFLAGS=-I. -g
LIBS = -lpthread -lssl
DEPS = ssl.h utils.h
CLIENT_OBJ = client.o
CLIENT_WITH_TCP_SOCKET = client_tcpsocket.o
MEM_OBJ = mem.o
READMEM_OBJ = read_mem.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: client client_tcp mem readmem

client_tcp: $(CLIENT_WITH_TCP_SOCKET)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

client: $(CLIENT_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

mem: $(MEM_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

readmem: $(READMEM_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f client client_tcp mem readmem *.o
