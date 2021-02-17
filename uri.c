
#include "uri.h"

#include <stdio.h>

#define MAX_PATH_SIZE 1024
const char* get_path_from_request(char* request) {
  UriUriA uri;
  const char* errorPos;

  if (uriParseSingleUriA(&uri, request, &errorPos) != URI_SUCCESS) {
    perror("URI PARSE FAILED");
    return NULL;
  }

  /* index */
  if (!uri.pathHead) {
    return "/index.gmi";
  }

  const char* path = uri.hostText.afterLast;

  uriFreeUriMembersA(&uri);
  return path;
}

