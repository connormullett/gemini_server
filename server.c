#include <arpa/inet.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1965
#define GEMINI_MAX_REQUEST_SIZE 1024

int create_socket(int port) {
  int s;
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("Unable to create socket");
    exit(EXIT_FAILURE);
  }

  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Unable to bind");
    exit(EXIT_FAILURE);
  }

  printf("Listening on port %d\n", PORT);
  if (listen(s, 1) < 0) {
    perror("Unable to listen");
    exit(EXIT_FAILURE);
  }

  return s;
}

void init_openssl() {
  SSL_load_error_strings();
  OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() { EVP_cleanup(); }

SSL_CTX *create_context() {
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  method = SSLv23_server_method();

  ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

void configure_context(SSL_CTX *ctx) {
  SSL_CTX_set_ecdh_auto(ctx, 1);

  /* Set the key and cert */
  if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

char *loadfile() {
  FILE *fp;
  long lSize;
  char *buffer;

  fp = fopen("contentroot/index.gmi", "rb");
  if (!fp) perror("blah.txt"), exit(1);

  fseek(fp, 0L, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);

  buffer = calloc(1, lSize + 1);
  if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

  if (1 != fread(buffer, lSize, 1, fp))
    fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

  fclose(fp);

  return buffer;
}

void handle_connection(SSL *ssl) {
  char *request = malloc(GEMINI_MAX_REQUEST_SIZE * sizeof(char));

  SSL_read(ssl, request, GEMINI_MAX_REQUEST_SIZE);

  char *header = "20 text/gemini";
  char *body = loadfile();
  size_t len_response = strlen(header) + strlen(body);
  char *response = malloc(sizeof(char) * len_response);
  memset(response, 0, len_response);
  sprintf(response, "%s\r\n%s\r\n", header, body);

  SSL_write(ssl, response, strlen(response));
}

int main(int argc, char **argv) {
  int sock;
  SSL_CTX *ctx;

  init_openssl();
  ctx = create_context();

  configure_context(ctx);

  sock = create_socket(PORT);

  while (1) {
    struct sockaddr_in addr;
    uint len = sizeof(addr);
    SSL *ssl;

    int client = accept(sock, (struct sockaddr *)&addr, &len);
    if (client < 0) {
      perror("Unable to accept");
      exit(EXIT_FAILURE);
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client);

    if (SSL_accept(ssl) <= 0) {
      ERR_print_errors_fp(stderr);
    } else {
      printf("request :: %d\n", client);
      handle_connection(ssl);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client);
  }

  close(sock);
  SSL_CTX_free(ctx);
  cleanup_openssl();
}
