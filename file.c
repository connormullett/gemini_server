
#include "file.h"

char *build_full_path(char *path) {
  char *content_root = "./contentroot";

  char *full_path = malloc(sizeof(char) * PATH_MAX);
  memset(full_path, 0, PATH_MAX);

  sprintf(full_path, "%s%s", content_root, path);
  return full_path;
}

bool file_exists(const char *path) {
  return access(path, F_OK) == 0 ? true : false;
}

ServerFile *create_server_file(const char *content, FILE_STATUS status) {
  ServerFile *server_file = malloc(sizeof(ServerFile));
  server_file->status = status;
  server_file->page_content = content;
  return server_file;
}

#define SERVER_FAIL NULL
char *read_file(char *path) {
  FILE *fp;
  long lSize;
  char *buffer;

  fp = fopen(path, "rb");
  if (!fp) {
    perror("[*] Error opening file");
    return SERVER_FAIL;
  }

  fseek(fp, 0L, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);

  buffer = calloc(1, lSize + 1);
  if (!buffer) {
    fclose(fp);
    fputs("[*] memory alloc fails", stderr);
    return SERVER_FAIL;
  }

  if (1 != fread(buffer, lSize, 1, fp)) {
    fclose(fp);
    free(buffer);
    fputs("[*] entire read fails", stderr);
    return SERVER_FAIL;
  }

  fclose(fp);
  return buffer;
}

ServerFile *loadfile(const char *path) {
  char *full_path = build_full_path((char *)path);

  if (!file_exists(full_path)) {
    fprintf(stderr, "[*] %s does not exist\n", full_path);
    printf("%s\n", strerror(errno));
    return create_server_file(NULL, ERR_NOT_FOUND);
  }

  printf("[*] request %s\n", path);

  char *buffer = read_file(full_path);

  if (!buffer) return create_server_file(NULL, SERVER_ERR);

  return create_server_file(buffer, OK);
}

