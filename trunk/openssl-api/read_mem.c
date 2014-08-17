#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"

// A READ BIO can be read repeatedly to get the same data
main() {

char data[] = "Hellow World";
BIO *mem;
BUF_MEM *bufMem;

ERR_load_BIO_strings();
SSL_load_error_strings();
OpenSSL_add_all_algorithms();

mem = BIO_new_mem_buf(data, -1);
BIO_get_mem_ptr(mem, &bufMem);

// Read out contents of Read BIO
printf("Reading contents of BIO first time: %s", bufMem->data);
printf("\n");
// Read out contents of Read BIO again
printf("Reading contents of BIO second time: %s", bufMem->data);
printf("\n");

BIO_set_close(mem, BIO_NOCLOSE); // leaves BUF_MEM allocated
BIO_free(mem);

}



