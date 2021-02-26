
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

char *get_last_name_in_path(char *path) {
  int init_size = strlen(path);
  char delim[] = "/";

  char *path_copy = malloc(sizeof(char) * init_size);
  strncpy(path_copy, path, init_size);

  char *cur_token = strtok(path_copy, delim);
  char *last;

  while (cur_token != NULL) {
    last = cur_token;
    cur_token = strtok(NULL, delim);
  }

  return last;
}

char *build_dir_response(char *path) {
  DIR *directory;
  struct dirent *dir;
  directory = opendir(path);

  char *content = malloc(sizeof(char) * PATH_MAX);
  memset(content, 0, strlen(content));

  char link[PATH_MAX] = {0};
  char *current_dir_short_name = get_last_name_in_path(path);

  char *content_path_header = malloc(sizeof(char) * PATH_MAX);

  if (directory) {
    sprintf(content_path_header, "# %s\n", current_dir_short_name);
    strncat(content, content_path_header, strlen(content_path_header));
    while ((dir = readdir(directory)) != NULL) {
      // => bar/baz.gmi baz.gmi
      sprintf(link, "=> %s/%s %s\n", current_dir_short_name, dir->d_name,
              dir->d_name);
      strncat(content, link, strlen(link));
    }
    closedir(directory);
  }

  return content;
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

