CC=gcc
CFLAGS=-I. -g
LIBS = -lpthread -lssl
DEPS = ssl.h utils.h
CLIENT_OBJ = client.o
MEM_OBJ = mem.o
READMEM_OBJ = read_mem.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: client mem readmem

client: $(CLIENT_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

mem: $(MEM_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

readmem: $(READMEM_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f client mem readmem *.o
