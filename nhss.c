#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "nhss.h"
#include "level.h"
#include "move.h"
#include "record.h"

nhss_info_t info;
extern int optind;
int boulders;  /* Set to 1 if boulders are to be represented as '0's */

void usage(const char *name) {
  printf("%s: missing argument\n", name);
  printf("Usage: %s [-rO] level [record]\n\n", name);
  printf("-r: replay recfile\n");
  printf("-O: display boulders as '0'\n");
  printf("level: level file, containing a nethack level\n");
  printf("record: file to hold a recording of the session\n");
}

int main(int argc, char **argv) {
  char *filename, *recfile = NULL;
  char movekey;
  int ret,replay=0;

  while ((ret = getopt(argc, argv, "rhO")) != -1) {
    switch(ret) {
      case 'r':
        replay=1;
        break;
      case 'O': boulders = 1; break;
      case 'h':
        usage(argv[0]);
        exit(E_SUCCESS);
        break;
    }
  }
  if (optind >= argc) {  // Make sure user passed a filename
    usage(argv[0]);
    exit(E_ERROR);
  }
  filename=argv[optind];
  if (optind+1 < argc) recfile=argv[optind+1];
  else ret = E_SUCCESS;  /* Just to be sure the `ret==E_ERROR' below isn't triggered */
  initscr();
  if(has_colors() == FALSE)
  {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_BLACK); // walls
  init_pair(2,COLOR_YELLOW,COLOR_BLACK); // holes
  cbreak();
  noecho();

  if (replay) {
    ret = recfile != NULL ? record_open(recfile, "r") : E_ERROR;
  }
  else if (recfile != NULL) {
    ret=record_open(recfile, "w");
  }
  if (ret==E_ERROR) {
    endwin();
    if (recfile == NULL) fprintf(stderr, "nhss: no record to replay\n");
    else fprintf(stderr, "nhss: could not open %s for recording\n", recfile);
    return E_ERROR;
  }

  ret=level_load(filename);  // Load the level
  if (ret==E_ERROR) {
    endwin();
    fprintf(stderr, "nhss: could not load level from %s\n", filename);
    return E_ERROR;
  }

  ret=level_findplayer();  // Search for the player
  if (ret==E_ERROR) {
    endwin();
    fprintf(stderr, "nhss: could not find player in level\n");
    return E_ERROR;
  }
  level_print();

  if (replay) {
    while (movekey=record_get()) {
      ret = moveKey(movekey);
      level_print();
      refresh();
      if (ret == E_ERROR) {
        endwin();
        record_close();
        fprintf(stderr, "nhss: bad recording\n");
      }
      if (getch() == 'q') {
        endwin();
        record_close();
        return E_SUCCESS;
      }
    }
  }
  else {
    while (movekey = getch()) {  // Read in a direction key
      if (movekey == 'q') {  // If the key is q, quit.
        endwin();
        record_close();  // This function /could/ return an error, but what am I going to do?  quit?  ;]
        return E_SUCCESS;
      }
      ret = moveKey(movekey);
      level_print();
      if (ret == E_ERROR) {  // If the key is bad, show an error
        statusline("Bad direction key");
      }
      else {
        statusline(NULL);
        record_add(movekey);  // If the key is valid, add it to the recording.
      }
    }
  }
  endwin();
}

void statusline(const char *line) {
  if (line == NULL) {
    move(info.lines, 0);
    clrtoeol();
  }
  else {
    beep();
    mvaddstr(info.lines, 0, line);
  }
  move(info.player[0], info.player[1]);
}
