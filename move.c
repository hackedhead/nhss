#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "nhss.h"
#include "move.h"

extern nhss_info_t info;
extern int boulders;

int moveKey(char dir) {
  switch (dir) {
    case 'k':
      moveto(0, 1);
      return E_SUCCESS;
    case 'j':
      moveto(0, -1);
      return E_SUCCESS;

    case 'l':
      moveto(1, 0);
      return E_SUCCESS;
    case 'h':
      moveto(-1, 0);
      return E_SUCCESS;

// diagonals are disabled here, as in sokoban they're not much use.
// You can't move diagonally around a boulder.
// We'd need to add rules in moveto to disallow moves between diag adjacent boulders and walls to enable this and have it behave correctly.
    case 'b':
      moveto(-1, -1);
      return E_SUCCESS;
    case 'y':
      moveto(-1, 1);
      return E_SUCCESS;

    case 'n':
      moveto(1, -1);
      return E_SUCCESS;
    case 'u':
      moveto(1, 1);
      return E_SUCCESS;


    default:
      return E_ERROR;
      break;
  }
}

// 1 if we are attempting a diagonal move, zero else
int isdiag(int x, int y) {
  return (abs(x) + abs(y)) - 1; 
}


void moveto(int x, int y) {	// Moves the character to the position specified by the coordinates, relative to the player
  switch (RELPOS(x, y)) {
    case '.': // an empty space
      switch (isdiag(x, y)) {
        case 1:
          if (!(RELPOS(x,0) == '.' || RELPOS(0,y) == '.')) {
            break; // if we find no empty space, fall through to no move.
          }
        default:
          POS = '.';	// Get rid of player at the old location
          RELPOS(x, y) = '@'; // Move player to new location
          info.player[0] = info.player[0] - y;  // update player location in info
          info.player[1] = info.player[1] + x;
        }
      break;
    case '`': // a boulder
    case '0':
      if (isdiag(x,y)) { 
        statusline("Boulders will not roll diagonally on this floor");
        break;
      }
      POS = '.';	// get rid of player
      info.player[0] = info.player[0] - y; // update player location so we can move the boulder
      info.player[1] = info.player[1] + x;
      switch (RELPOS(x, y)) {	// What are we trying to push the boulder into?
        case '.':
          POS = '@';	// get rid of boulder, add player
          RELPOS(x, y) = boulders ? '0': '`';
        break;
        case '^': // filling in a pit, woohoo!
          POS = '@';
          RELPOS(x, y) = '.';
          break;
        default:
          info.player[0] = info.player[0] + y; // Just kidding, not moving.  Reset player info.
          info.player[1] = info.player[1] - x;
          POS = '@';
        break;
      }
    break;
  }
}
