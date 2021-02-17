
#include "response.h"

void send_server_error_response(SSL* ssl) {
  const char* response = "50 INTERNAL SERVER ERROR\r\n";
  printf("[*] %s\n", response);
  SSL_write(ssl, response, strlen(response));
}

void send_bad_request_response(SSL* ssl) {
  const char* response = "59 BAD REQUEST\r\n";
  printf("[*] %s\n", response);
  SSL_write(ssl, response, strlen(response));
}

void send_not_found_response(SSL* ssl) {
  const char* response = "51 NOT FOUND\r\n";
  printf("[*] %s\n", response);
  SSL_write(ssl, response, strlen(response));
}

void send_ok_response(SSL* ssl, ServerFile* server_file) {
  const char* header = "20 text/gemini";
  size_t len_response = strlen(header) + strlen(server_file->page_content);

  char* response = malloc(sizeof(char) * len_response);
  sprintf(response, "%s\r\n%s\r\n", header, server_file->page_content);

  printf("[*] %s\n", header);
  SSL_write(ssl, response, strlen(response));

  free((char*)server_file->page_content);
  free(server_file);
}

