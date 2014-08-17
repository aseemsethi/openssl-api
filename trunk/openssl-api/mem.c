/*
 * For testing purposes, compiled as follows:
 * gcc -g mem.c -L/usr/home/aseem/bin/lib/ -lssl -lcrypto -I/usr/home/aseem/bin/include/ -ldl
 * Else, 
 *     run "make" and it wil compile all the files, and generate "mem"
 *
 * Used openssl-1.0.1h and compiled with "./config -d --prefix=/usr/home/aseem/bin; make install"
 *
 */

#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"

main() {
BIO *mem;
int ret;

ERR_load_BIO_strings();
SSL_load_error_strings();
OpenSSL_add_all_algorithms();

mem = BIO_new(BIO_s_mem());

// Example - 1: Write into Buffer
// BIO_puts writes a NULL terminated string into memory
BIO_puts(mem, "Hello Aseem Sethi");  // BIO_puts points to mem_write

// Read the buffer back via BIO_gets()
BUF_MEM *bufMem;
char data[100], data1[100];
ret = BIO_gets(mem, data, 100);  // points to mem_gets =>mem_read
printf("\nBuffer Read Data: ret=%d: %s", ret, data);

//Try to Read the data again. Data once read is deleted
ret = BIO_gets(mem, data1, 100); // result is always null terminated
printf("\nBuffer Read Data Again: ret=%d: %s", ret, data1);
printf("\n..........................");

// Example - 2: Write into Buffer
BIO_puts(mem, "Bye..");  // points to mem_write

// Read the buffer back via BIO_get_mem_data()
// This is not null terminated. Read only what you need.
char *buff = NULL;
size_t len = BIO_get_mem_data(mem, &buff);
printf("\nBuffer Read Data via get_mem_data: length=%d: %.*s", len, len, buff);
printf("\n strlen: %d", strlen(buff));
printf("\n..........................\n");
}
