/*
 * This program is a simple SSL Client that does a HTTP GET from a server, 
 * running using "openssl s_server -WWW command.
 * Code modifed from savetheions.com/2010/01/16/
 * code example for using OpenSSL API.
 * This example uses an SSL socket that connects to a native TCP socket in 
 * Linux
 */
#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"

#define SERVER "www.openssl.org"
#define PORT 443

typedef struct {
	int socket;
	SSL *sslHandle;
	SSL_CTX *ctx;
} connection;

tcpConnect() {
	int error, handle;
	struct hostent *host;
	struct sockaddr_in server;

	host = gethostbyname(SERVER);
	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1) {
		perror("Socket:"); handle = 0; exit(1);
	} else {
		server.sin_family = AF_INET;
		server.sin_port = htons(PORT);
		server.sin_addr = *((struct in_addr*) host->h_addr);
		bzero(&(server.sin_zero),8);
		error = connect(handle, (struct sockaddr*)&server,
					sizeof(struct sockaddr));	
		if (error == -1) {
			perror("Connect:"); handle = 0; exit(1);
		}
	}
	return handle;
}

int main() {
	SSL	*ssl;
	int read, write;
	connection *c;
	char *request = "GET /index.html HTTP/1.1\x0D\x0AHost: 127.0.0.1\x0D\x0A\x0D\x0A";
	char response[1024];

	c = malloc(sizeof(connection));
	c->sslHandle = NULL;
	c->ctx = NULL;
	c->socket = tcpConnect();

	// Setup the library
	ERR_load_BIO_strings();
	SSL_load_error_strings(); // loads both crypto and SSL error strings
	OpenSSL_add_all_algorithms(); // adds all - including _ciphers and _digests

	// SSL_library_init() - ssl/ssl_algs.c
	// Adds ciphers - EVP_add_digest(EVP_md5, etc)
	// Loads ciphers into array ssl_cipher_methods[] - ssl_load_ciphers()
	SSL_library_init(); // important to do this initialization

	// Set up the SSL Context. SSL_CTX is of type ssl_ctx_st
	// Members: method=>SSLv23_client_method(), session_cache, stats, callback functions.
	c->ctx = SSL_CTX_new(SSLv23_client_method());

	// Load the trust store
	if (!(SSL_CTX_load_verify_locations(c->ctx, "./webserver.crt", NULL))) {
		printf("\n Error loading trust store");
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(c->ctx);
		return 0;
	}

	// Set up the BIO chain with SSL BIO and set it's FD to our socket
	c->sslHandle = SSL_new(c->ctx);
	if(c->sslHandle == NULL)
		ERR_print_errors_fp(stderr);
	if (!SSL_set_fd(c->sslHandle, c->socket))
		ERR_print_errors_fp(stderr);
	if(SSL_connect(c->sslHandle) != -1)
		ERR_print_errors_fp(stderr);

	printf("\nConnected to Web Server");

#ifdef COMMENT 
	// Check the certificate
	if(SSL_get_verify_result(c->sslHandle) != X509_V_OK) {
		printf("Certificate verification error: %d\n",
				SSL_get_verify_result(ssl));
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(c->ctx);
		return 0;
	}
	printf("Certificate verification passed\n");
#endif

	// Send the request
	printf("\nRequest Length: %d", strlen(request));
	write = SSL_write(c->sslHandle, request, strlen(request));
	if (write <= 0) {
		printf("\n SSL_write writes <= 0 data");
		// Can retry here if BIO_should_retry(bio) is true
		exit(1);
	}
	for (;;) {
		read = SSL_read(c->sslHandle, response, 1023);
		if (read < 0) { printf("\nBIO_read return < 0"); break; } // Can retry
		if(read == 0) { printf("\nBIO_read return 0"); break; } // No more data
		response[read] = "\0";
		printf("\nResponse: %s", response);
	}
	fflush(stdout);
	SSL_CTX_free(c->ctx);
	return 0;
}
