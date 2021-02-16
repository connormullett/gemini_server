
#include "uri.h"

#include <stdio.h>

#define MAX_PATH_SIZE 1024
char* get_path_from_request(char* request) {
  UriUriA uri;
  const char* errorPos;

  if (uriParseSingleUriA(&uri, request, &errorPos) != URI_SUCCESS) {
    perror("URI PARSE FAILED");
    return NULL;
  }

  /* index */
  if (!uri.pathHead) {
    return "index.gmi";
  }

  char* path = malloc(sizeof(char) * MAX_PATH_SIZE);
  UriPathSegmentA* head = uri.pathHead;

  while (head != NULL) {
    /* puts(head->text.first); */
    head = head->next;
  }

  uriFreeUriMembersA(&uri);
  return path;
}

