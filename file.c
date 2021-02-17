
#include "file.h"

bool file_exists(const char *path) {
  char *content_root = "./contentroot";

  size_t len_full_path = (strlen(content_root) + strlen(path));
  char *full_path = malloc(sizeof(char) * len_full_path);
  memset(full_path, 0, len_full_path);

  sprintf(full_path, "%s%s", content_root, path);

  puts(full_path);

  free(full_path);
  return true;
}

ServerFile *create_server_file(const char *content, FILE_STATUS status) {
  ServerFile *server_file = malloc(sizeof(ServerFile));
  server_file->status = status;
  server_file->page_content = content;
  return server_file;
}

ServerFile *loadfile(const char *path) {
  FILE *fp;
  long lSize;
  char *buffer;

  if (!file_exists(path)) {
    perror("file does not exist");
    return create_server_file(NULL, ERR_NOT_FOUND);
  }

  fp = fopen(path, "rb");
  if (!fp) perror("Error opening file"), exit(1);

  fseek(fp, 0L, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);

  buffer = calloc(1, lSize + 1);
  if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

  if (1 != fread(buffer, lSize, 1, fp))
    fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

  fclose(fp);

  return create_server_file(buffer, OK);
}
