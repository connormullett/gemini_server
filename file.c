
#include "file.h"

char *loadfile(const char *path) {
  FILE *fp;
  long lSize;
  char *buffer;

  printf("PATH :: %s\n", path);

  fp = fopen("contentroot/index.gmi", "rb");
  if (!fp) perror("blah.txt"), exit(1);

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
