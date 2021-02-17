
#include "file.h"

char *build_full_path(char *path) {
  char *content_root = "./contentroot";

  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  printf("CWD :: %s\n", cwd);

  char *full_path = malloc(sizeof(char) * PATH_MAX);
  memset(full_path, 0, PATH_MAX);

  sprintf(full_path, "%s%s", content_root, path);
  return full_path;
}

bool file_exists(const char *path) { return access(path, F_OK) ? true : false; }

ServerFile *create_server_file(const char *content, FILE_STATUS status) {
  ServerFile *server_file = malloc(sizeof(ServerFile));
  server_file->status = status;
  server_file->page_content = content;
  return server_file;
}

char *read_file(char *path) {
  FILE *fp;
  long lSize;
  char *buffer;

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
  return buffer;
}

ServerFile *loadfile(const char *path) {
  char *full_path = build_full_path((char *)path);

  if (!file_exists(full_path)) {
    perror("file does not exist");
    return create_server_file(NULL, ERR_NOT_FOUND);
  }

  char *buffer = read_file(full_path);

  return create_server_file(buffer, OK);
}

