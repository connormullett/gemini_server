#ifndef __FILE_H
#define __FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum file_status { OK, ERR_NOT_FOUND };
struct server_file {
  const char* page_content;
  enum file_status status;
};
typedef enum file_status FILE_STATUS;
typedef struct server_file ServerFile;

bool file_exists(const char* path);
ServerFile* create_server_file(const char* content, FILE_STATUS status);
ServerFile* loadfile(const char* path);

#endif
