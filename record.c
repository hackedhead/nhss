#include <stdio.h>
#include <errno.h>
#include "nhss.h"

extern nhss_info_t info;

int record_open(const char *filename, const char *mode) {
  info.recfile = fopen(filename, mode);
  if (info.recfile == NULL) {
    fprintf(stderr, "nhss: fopen(): %s at %s:%d\n", strerror(errno), __FILE__, __LINE__);
    return E_ERROR;
  }
  return E_SUCCESS;
}

int record_add(const char dir) {
  if (info.recfile == NULL) return E_SUCCESS;
  int ret;
  ret=fputc(dir, info.recfile);
  if (ret == EOF) {
    fprintf(stderr, "nhss: fputc(): %s at %s:%d\n", strerror(errno), __FILE__, __LINE__);
    return E_ERROR;
  }
  return E_SUCCESS;
}

int record_close() {
  if (info.recfile == NULL) return E_SUCCESS;
  int ret;
  ret = fclose(info.recfile);
  if (ret) {
    fprintf(stderr, "nhss: fclose(): %s at %s:%d\n", strerror(errno), __FILE__, __LINE__);
    return E_ERROR;
  }
  return E_SUCCESS;
}

char record_get() {
  int ret;
  ret=fgetc(info.recfile);
  if (ret == EOF) {
    return 0;	// No E_ERROR stuff here.  It's not really an error.  just want to stop replaying.
  }
  else {
    return ret;
  }
}
