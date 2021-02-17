#ifndef __FILE_H
#define __FILE_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum file_status { OK, ERR_NOT_FOUND };
struct server_file {
  const char* page_content;
  enum file_status status;
};
typedef enum file_status FILE_STATUS;
typedef struct server_file ServerFile;

char* build_full_path(char* path);
bool file_exists(const char* path);
ServerFile* create_server_file(const char* content, FILE_STATUS status);
char* read_file(char* path);
ServerFile* loadfile(const char* path);

#endif
