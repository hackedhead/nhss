#ifndef NHSS_H
#define NHSS_H

#define E_ERROR 1
#define E_SUCCESS 0

typedef struct nhss_info {
	char *level[24];	// Contains text version of level
	unsigned int lines;	// Actual number of lines in the level

	unsigned int player[2];		// Player position.  [0] == line number, [1] == index on line
	FILE *recfile;		// file to record movement to
} nhss_info_t;

#define POS info.level[info.player[0]][info.player[1]]
#define RELPOS(x, y) info.level[info.player[0] - y][info.player[1] + x]

// RELPOS == Relative Position.  Returns the character in the position specified by the coordinates, relative to the player.

void statusline(const char *line);

#endif
