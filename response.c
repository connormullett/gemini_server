
#include "response.h"

void send_not_found_response(SSL* ssl) {
  const char* not_found = "51\r\n";
  SSL_write(ssl, not_found, strlen(not_found));
}

void send_ok_response(SSL* ssl, ServerFile* server_file) {
  const char* header = "20 text/gemini";
  size_t len_response = strlen(header) + strlen(server_file->page_content);
  char* response = malloc(sizeof(char) * len_response);
  sprintf(response, "%s\r\n%s\r\n", header, server_file->page_content);
  free((char*)server_file->page_content);
  free(server_file);
}

