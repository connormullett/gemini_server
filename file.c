
#include "file.h"

char *build_full_path(char *path) {
  extern const char *content_root;

  char *full_path = malloc(sizeof(char) * PATH_MAX);
  memset(full_path, 0, PATH_MAX);

  sprintf(full_path, "%s%s", content_root, path);
  return full_path;
}

bool file_exists(const char *path) {
  return access(path, F_OK) == 0 ? true : false;
}

int is_file(const char *path) {
  struct stat statbuf;
  if (stat(path, &statbuf) != 0) return 0;
  return S_ISDIR(statbuf.st_mode);
}

char *build_dir_response(char *path) {
  DIR *directory;

  struct dirent *dir;
  directory = opendir(path);

  char *content = malloc(sizeof(char) * 1024);
  char *file_name = malloc(sizeof(char) * 1024);

  if (directory) {
    while ((dir = readdir(directory)) != NULL) {
      memset(file_name, 0, 1024);
      sprintf(file_name, "=> gemini://localhost/%s %s\n", path, dir->d_name);
      file_name[strlen(dir->d_name)] = 0;
      printf("file name :: %s\n", file_name);
      strncat(content, file_name, strlen(file_name));
    }
    closedir(directory);
  }

  printf("Content :: %s\n", content);

  return NULL;
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
    perror("[*] memory alloc fails");
    return SERVER_FAIL;
  }

  if (1 != fread(buffer, lSize, 1, fp)) {
    fclose(fp);
    free(buffer);
    perror("[*] entire read fails\n");
    return SERVER_FAIL;
  }

  fclose(fp);

  return buffer;
}

ServerFile *loadfile(const char *path) {
  char *full_path = build_full_path((char *)path);

  if (!file_exists(full_path)) {
    fprintf(stderr, "[*] %s does not exist\n", full_path);
    return create_server_file(NULL, ERR_NOT_FOUND);
  }

  if (is_file(full_path)) {
    char *content = build_dir_response(full_path);
    return create_server_file(content, OK);
  }

  printf("[*] request %s\n", path);

  char *buffer = read_file(full_path);

  if (buffer == SERVER_FAIL) return create_server_file(NULL, SERVER_ERR);

  return create_server_file(buffer, OK);
}

