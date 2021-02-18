#ifndef __FILE_H
#define __FILE_H

#include <dirent.h>
#include <errno.h>
#include <libconfig.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

enum file_status { OK, ERR_NOT_FOUND, SERVER_ERR };
struct server_file {
  const char* page_content;
  enum file_status status;
};
typedef enum file_status FILE_STATUS;
typedef struct server_file ServerFile;

int is_dir(const char* path);
char* build_dir_response(char* path);
char* build_full_path(char* path);
bool file_exists(const char* path);
ServerFile* create_server_file(const char* content, FILE_STATUS status);
char* read_file(char* path);
ServerFile* loadfile(const char* path);

#endif
