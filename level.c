#include <ncurses.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "nhss.h"
#include "level.h"

extern nhss_info_t info;
extern int boulders;

int level_load(const char *filename) {
  FILE *levelfile;
  int i;	// counters for loops

  char line[81];

  levelfile = fopen(filename, "r");
  if (levelfile == NULL) {
    fprintf(stderr, "nhss: fopen(): %s at %s:%d\n", strerror(errno), __FILE__, __LINE__);
    return E_ERROR;
  }

  for (i=0;i<24, !feof(levelfile);i++) { // Run until i is 24 (i==line), or until EOF.
    if (fgets(line, 81, levelfile) == NULL) {	// Read in one line
      if (!feof(levelfile)) {	// If there was an error, and it is /not/ an EOF, yell about it
        fprintf(stderr, "nhss: fgets(): %s at %s:%d\n", strerror(errno), __FILE__, __LINE__);
        return E_ERROR;
      }
    }
    if (feof(levelfile)) {
      break;
    }
    char* lp;
    for (lp = line; *lp != '\n'; lp++) {
     if (*lp == '\0') {  /* The line doesn't contain a newline and so is part of a line greater than 80 characters in length. */
      int ch = 0;
      while (ch != '\n' && ch != EOF) ch = fgetc(levelfile);
      if (ferror(levelfile)) {
       fprintf(stderr, "nhss: fgetc(): %s at %s:%d\n", strerror(errno), __FILE__, __LINE__);
       return E_ERROR;
      }
      break;
     } else if (strchr("|-+`0@.^<>* ", *lp) == NULL) *lp = '.';
     else if (*lp == '0' && !boulders) *lp = '`';
     else if (*lp == '`' && boulders) *lp = '0';
    }
    info.level[i]=strdup(line);      // 'line' gets reused, so copy it first
  }
  info.lines = i;
  fclose(levelfile);

  return E_SUCCESS;
}

int level_print() {
  int i,j;	// counters
  for (i=0;i<info.lines;i++) {	// loop through the array and print all the lines
    //mvaddstr(i, 0, info.level[i]);	// the newline is included in the line.
    int line_len = strlen(info.level[i]);
    for (j=0;j<line_len;j++) {
      // print each char, add colors
      if ( info.level[i][j] == '-' || info.level[i][j] == '|' ) {
        attron(COLOR_PAIR(1));
      }
      if ( info.level[i][j] == '^' ) {
        attron(COLOR_PAIR(2));
      }
      mvaddch(i, j, info.level[i][j]);	// the newline is included in the line.
      if ( info.level[i][j] == '-' || info.level[i][j] == '|' ) {
        attroff(COLOR_PAIR(1));
      }
      if ( info.level[i][j] == '^' ) {
        attroff(COLOR_PAIR(2));
      }
    }
  }
  move(info.player[0], info.player[1]);
  return E_SUCCESS;
}

int level_findplayer() {
  int i;	// counter
  char *ret=NULL;	// null until the hero is found.
  for (i=0;i<info.lines && ret==NULL;i++) {	// loop through the array, and search each line for player.
    ret=strchr(info.level[i], '@');
  }
  // 'i++' gets run after every iteration of the loop, so subtract one.
  if (ret!=NULL) {
    info.player[0] = i-1;
    info.player[1] = ret - info.level[i-1];
    for (i=0;i<2;i++) {
      info.stairs[i] = info.player[i];
    }
    return E_SUCCESS;
  }
  else {
    return E_ERROR;	// Could not find player.
  }
}
