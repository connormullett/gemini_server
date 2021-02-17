
#ifndef __RESPONSE_H
#define __RESPONSE_H

#include <openssl/err.h>
#include <openssl/ssl.h>
#include <string.h>

#include "file.h"

void send_not_found_response(SSL* ssl);
void send_ok_response(SSL* ssl, ServerFile* server_file);

#endif

