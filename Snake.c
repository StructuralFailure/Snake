#include "Snake.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define START_LENGTH 3
#define COL_MIN_LENGTH 5 /* minimum snake length where a collision can occur */

#define STATE_ALIVE 0
#define STATE_LOST  -1
#define STATE_WON   1

typedef struct Pos Pos;

void Snake_place_apple(Snake* s);
void Snake_print_parts(Snake* s); /* debug function */
int contain_index(int index, int size); /* safely handles over- and underflow of indices */

struct Snake {
	/* general state */
	int width;
	int height;
	int state;

	/* protagonist's state */
	int direction;
	int last_direction;
	int length;
	int head_offset;    /* index of head position in position buffer */
	int pos_size;
	Pos* pos;           /* circular buffer containing positions of all snake parts */
	Pos* apple_pos;     /* position of apple */
};

struct Pos {
	int x;
	int y;
};

/* exposed functions */

Snake* Snake_create(int width, int height) {
	Snake* s;
	s = malloc(sizeof(Snake));
	if (!s) {
		return NULL;
	}

	/* seed randomizer */
	srand(time(NULL));

	/* initialize struct */
	s->width = width;
	s->height = height;
	s->state = STATE_ALIVE;

	s->direction = DIR_RIGHT;
	s->last_direction = DIR_RIGHT;
	s->length = START_LENGTH;
	s->head_offset = 0;
	s->pos_size = width * height;

	s->pos = malloc(sizeof(Pos) * s->pos_size);
	s->apple_pos = malloc(sizeof(Pos));
	if (!s->pos || !s->apple_pos) {
		return NULL;
	}

	/* place protagonist and apple */
	int center_x = width / 2;
	int center_y = height / 2;
	for (int i = 0; i < s->length; ++i) {
		s->pos[s->head_offset].x = center_x + i;
		s->pos[s->head_offset].y = center_y;

		//printf("[%d; %d] ", s->pos[s->head_offset].x, s->pos[s->head_offset].y);

		++s->head_offset;
	}
	--s->head_offset;

	Snake_place_apple(s);

	return s;
}

void Snake_destroy(Snake* s) {
	free(s->pos);
	free(s->apple_pos);
	free(s);
}

int Snake_turn(Snake* s, int direction) {
	if (direction < 0 || direction > 3) {
		return TURN_INVALID;
	}

	/* snake can't do 180° turns */
	if ((s->last_direction + direction) % 2 == 0) {
		return TURN_NO_180;
	}

	/* update direction */
	s->direction = direction;
	return TURN_OK;
}

int Snake_score(Snake* s) {
	return (s->length - START_LENGTH);
}

int Snake_update(Snake* s) {
	if (s->state == STATE_LOST) {
		return UPDATE_LOST;
	} else if (s->state == STATE_WON) {
		return UPDATE_WON;
	}

	/* save some typing later on*/
	Pos* p = s->pos;

	/* move and update head position's index in buffer */
	int new_head_offset = (s->head_offset + 1) % s->pos_size;
	
	switch (s->direction) {
	case DIR_LEFT:
		p[new_head_offset].x = contain_index(p[s->head_offset].x - 1, s->width);
		p[new_head_offset].y = p[s->head_offset].y;
		break;
	case DIR_UP:
		p[new_head_offset].x = p[s->head_offset].x;
		p[new_head_offset].y = contain_index(p[s->head_offset].y - 1, s->height);
		break;
	case DIR_RIGHT:
		p[new_head_offset].x = contain_index(p[s->head_offset].x + 1, s->width);
		p[new_head_offset].y = p[s->head_offset].y;
		break;
	case DIR_DOWN:
		p[new_head_offset].x = p[s->head_offset].x;
		p[new_head_offset].y = contain_index(p[s->head_offset].y + 1, s->height);
		break;
	}

	s->last_direction = s->direction;
	s->head_offset = new_head_offset;

	/* check for collisions */
	int part_index_start = contain_index(s->head_offset - s->length, s->pos_size);
	/* subtract 3 because the first three body parts cannot collide with the head */
	for (int i = 1; i < s->length - 3; ++i) {
		int part_index = (part_index_start + i) % s->pos_size;

		if (p[s->head_offset].x == p[part_index].x
		 && p[s->head_offset].y == p[part_index].y) {
			/* head's position is the same as at least one
			 * body part's position: collision and death   
			 */
			s->state = STATE_LOST;
			return UPDATE_DIE;
		}
	}
	return UPDATE_MOVE;
}

/* unexposed functions */

void Snake_place_apple(Snake* s) {
	int x;
	int y;
	int inside_snake;

	do {
		x = rand() % s->width;
		y = rand() % s->height;

		/* check whether apple was placed inside snake */
		/* TODO: improve algorithm; this could get
		 *       really slow as the snake grows long */
		inside_snake = 0;
		int part_index_start = contain_index(s->head_offset - s->length, s->pos_size);

		for (int i = 1; i <= s->length; ++i) {
			int part_index = (part_index_start + i) % s->pos_size;

			if (s->pos[part_index].x == x
			 && s->pos[part_index].y == y) {
				inside_snake = 1;
				break;
			} 
		}
	} while (inside_snake);

	s->apple_pos->x = x;
	s->apple_pos->y = y;
}

/* debug function */
void Snake_print_parts(Snake* s) {
	int part_index_start = contain_index(s->head_offset - s->length, s->pos_size);

	for (int i = 1; i <= s->length; ++i) {
		int part_index = (part_index_start + i) % s->pos_size;
		printf("@ %d: [%d; %d]\n", part_index, s->pos[part_index].x, s->pos[part_index].y);
	}
}

/* helper function that wraps around "index" 
 * when it reaches "size" or goes below 0
 */
int contain_index(int index, int size) {
	return ((index % size) + size) % size;
}