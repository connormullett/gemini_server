#include <arpa/inet.h>
#include <libconfig.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "file.h"
#include "response.h"
#include "uri.h"

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

  printf("Listening on port %d\n", port);
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

#define GEMINI_MAX_REQUEST_SIZE 1024
void handle_connection(SSL *ssl) {
  char *request = malloc(GEMINI_MAX_REQUEST_SIZE * sizeof(char));
  memset(request, 0, GEMINI_MAX_REQUEST_SIZE);

  SSL_read(ssl, request, GEMINI_MAX_REQUEST_SIZE);

  /* strip request */
  request[strcspn(request, "\r\n")] = 0;

  const char *path = get_path_from_request(request);

  ServerFile *server_file = loadfile(path);

  if (server_file->status == SERVER_ERR) {
    return send_server_error_response(ssl);
  }

  if (server_file->status == ERR_NOT_FOUND) return send_not_found_response(ssl);

  send_ok_response(ssl, server_file);
}

config_t *create_config() {
  config_t cfg, *cf;

  cf = &cfg;
  config_init(cf);

  if (!config_read_file(cf, "./server.cfg")) {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(cf),
            config_error_line(cf), config_error_text(cf));
    config_destroy(cf);
    exit(EXIT_FAILURE);
  }
  return cf;
}

int main(int argc, char **argv) {
  int sock;
  int port;
  SSL_CTX *ctx;

  init_openssl();
  ctx = create_context();

  configure_context(ctx);

  config_t *config = create_config();
  if (!config_lookup_int(config, "port", &port)) {
    printf("port :: %d\n", port);
    puts("[*] port not found in server.cfg"), exit(EXIT_FAILURE);
  }

  sock = create_socket(port);

  while (1) {
    struct sockaddr_in addr;
    uint len = sizeof(addr);
    SSL *ssl;

    int client = accept(sock, (struct sockaddr *)&addr, &len);
    if (client < 0) {
      perror("[*] Unable to accept");
      exit(EXIT_FAILURE);
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client);

    if (SSL_accept(ssl) <= 0) {
      ERR_print_errors_fp(stderr);
    } else {
      handle_connection(ssl);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client);
  }

  config_destroy(config);
  close(sock);
  SSL_CTX_free(ctx);
  cleanup_openssl();
}
