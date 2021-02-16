
#include "uri.h"

UriPathSegmentA* get_path_from_request(char* request) {
  UriUriA uri;
  const char* errorPos;

  if (uriParseSingleUriA(&uri, request, &errorPos) != URI_SUCCESS) {
    perror("URI PARSE FAILED");
    return NULL;
  }

  UriPathSegmentA* head = uri.pathHead;

  printf("PATH HEAD :: %s\n", head->text.first);

  return NULL;
}

