#ifndef RECORD_H
#define RECORD_H

int record_open(const char *filename, const char *mode);
int record_add(const char dir);
int record_close();
char record_get();

#endif
