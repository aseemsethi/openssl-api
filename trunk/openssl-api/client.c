/*
 * This program is a simple SSL Client that does a HTTP GET from a server, 
 * running using "openssl s_server -WWW command.
 * Code modifed from linux.die.net/man/3/bio_new_ssl_connect
 * code example for using OpenSSL API.
 */
#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"
#include "stdio.h"
#include "string.h"

int main() {
	BIO *bio;
	SSL	*ssl;
	SSL_CTX	*ctx;
	int read, write;
	
	char *request = "GET /index.html HTTP/1.1\x0D\x0AHost: 127.0.0.1\x0D\x0A\x0D\x0A";
	char response[1024];

	// Setup the library
	ERR_load_BIO_strings();
	SSL_load_error_strings(); // loads both crypto and SSL error strings
	OpenSSL_add_all_algorithms(); // adds all - including _ciphers and _digests

	SSL_library_init(); // important to do this initialization

	// Set up the SSL Context
	ctx = SSL_CTX_new(SSLv23_client_method());

	// Load the trust store
	if (!(SSL_CTX_load_verify_locations(ctx, "./webserver.crt", NULL))) {
		printf("\n Error loading trust store");
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(ctx);
		return 0;
	}

	// Set up the BIO chain with SSL BIO and Connect BIO
	bio = BIO_new_ssl_connect(ctx);
    /*
	 * Bio_new_ssl_coonect() explained below:
	 *
	 * 1) con=BIO_new(BIO_s_connect())
	 * BIO_new() mallocs BIO struct, sets BIO->method = methods_connectp
	 * 			BIO_s_connect returns pointer to methods_connectp { }
	 * 				- crypto/bio/bss_conn.c
	 *
	 * 2) ssl=BIO_new_ssl(ctx,1)
	 * BIO_new(BIO_f_ssl()) mallocs BIO struct, sets BIO->method = methods_sslp
	 *			BIO_f_ssl returns a pointer to methods_sslp { }
	 * 				- bio/bio_ssl.c
	 *
	 * 3) ret=BIO_push(ssl,con)
	 * 		sets ssl->next_bio = con
	 * 		return ret
	 */


	// Set the SSL_MODE_AUTO_RETRY flag
	BIO_get_ssl(bio, &ssl);
	if(!ssl) {
		printf("\n Could not get SSL pointer");
		exit(1);
	}
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
	// Create and setup the connection
	BIO_set_conn_hostname(bio, "127.0.01:4433");
	if(BIO_do_connect(bio) <= 0) {
		printf("\nError attempting to connect");
		ERR_print_errors_fp(stderr);
		BIO_free_all(bio);
		SSL_CTX_free(ctx);
		return 0;
	}
	printf("\nConnected to Web Server");

	// Check the certificate
	if(SSL_get_verify_result(ssl) != X509_V_OK) {
		printf("Certificate verification error: %d\n",
				SSL_get_verify_result(ssl));
		BIO_free_all(bio);
		SSL_CTX_free(ctx);
		return 0;
	}

	// Send the request
	printf("\nRequest Length: %d", strlen(request));
	write = BIO_write(bio, request, strlen(request));
	if (write <= 0) {
		printf("\n BIO_write writes <= 0 data");
		// Can retry here if BIO_should_retry(bio) is true
		exit(1);
	}
	for (;;) {
		read = BIO_read(bio, response, 1023);
		if (read < 0) { printf("\nBIO_read return < 0"); break; } // Can retry
		if(read == 0) { printf("\nBIO_read return 0"); break; } // No more data
		response[read] = "\0";
		printf("\nResponse: %s", response);
	}
	fflush(stdout);
	BIO_free_all(bio);
	SSL_CTX_free(ctx);
	return 0;
}
