#include "Snake.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define START_LENGTH 5
#define COL_MIN_LENGTH 5 /* minimum snake length where a collision can occur */

void Snake_place_apple(Snake* s);
void Snake_print_parts(Snake* s); /* debug function */
int contain_index(int index, int size); /* safely handles over- and underflow of indices */

struct Snake {
	/* general state */
	int width;
	int height;
	State state;

	/* protagonist's state */
	int direction;
	int last_direction;
	int length;
	int head_offset;    /* index of head position in position buffer */
	int pos_size;
	Pos* pos;           /* circular buffer containing positions of all snake parts */
	Pos* apple_pos;     /* position of apple */
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
	s->state = STATE_PLAYING;

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

State Snake_state(Snake* s) {
	return s->state;
}

Pos Snake_apple(Snake* s) {
	return *(s->apple_pos);
}

Turn Snake_turn(Snake* s, Dir direction) {
	if (direction > 3) {
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

Update Snake_update(Snake* s) {
	if (s->state == STATE_LOST) {
		return UPDATE_LOST;
	} else if (s->state == STATE_WON) {
		return UPDATE_WON;
	}

	/* save some typing later on*/
	Pos* p = s->pos;

	int new_head_offset = (s->head_offset + 1) % s->pos_size;
	Pos new_head_pos;

	switch (s->direction) {
	case DIR_LEFT:
		new_head_pos.x = contain_index(p[s->head_offset].x - 1, s->width);
		new_head_pos.y = p[s->head_offset].y;
		break;
	case DIR_UP:
		new_head_pos.x = p[s->head_offset].x;
		new_head_pos.y = contain_index(p[s->head_offset].y - 1, s->height);
		break;
	case DIR_RIGHT:
		new_head_pos.x = contain_index(p[s->head_offset].x + 1, s->width);
		new_head_pos.y = p[s->head_offset].y;
		break;
	case DIR_DOWN:
		new_head_pos.x = p[s->head_offset].x;
		new_head_pos.y = contain_index(p[s->head_offset].y + 1, s->height);
		break;
	}

	/* do collision check for new head offset 
	 * ignore back-most cell because it will be moved forward/to the side when this function finishes is over 
	 * ignore first three cells because the head cannot physically collide with them
	 */
	int part_index_start = contain_index(new_head_offset - s->length, s->pos_size);
	for (int i = 1; i < s->length - 3; ++i) {
		int part_index = (part_index_start + i) % s->pos_size;

		if (new_head_pos.x == p[part_index].x
		 && new_head_pos.y == p[part_index].y) {
			s->state = STATE_LOST;
			return UPDATE_DIE;
		}
	}

	p[new_head_offset] = new_head_pos;
	s->last_direction = s->direction;
	s->head_offset = new_head_offset;

	/* check for apple */
	if (new_head_pos.x == s->apple_pos->x
	 && new_head_pos.y == s->apple_pos->y) {
		++s->length;
		Snake_place_apple(s);
		return UPDATE_EAT;
	}

	/* no collision, update head position and direction */
	return UPDATE_MOVE;
}

int Snake_length(Snake* s) {
	return s->length;
}

Pos Snake_position(Snake* s, int i) {
	int real_index = contain_index(s->head_offset - s->length + 1 + i, s->pos_size);
	return s->pos[real_index];
}

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

/* debug functions */
/*void Snake_print_parts(Snake* s) {
	int part_index_start = contain_index(s->head_offset - s->length, s->pos_size);

	for (int i = 1; i <= s->length; ++i) {
		int part_index = (part_index_start + i) % s->pos_size;
		printf("@ %d: [%d; %d]\n", part_index, s->pos[part_index].x, s->pos[part_index].y);
	}
}*/

void Snake_set_length(Snake* s, int length) {
	s->length = length;
}


/* helper function that wraps around "index" 
 * when it reaches "size" or goes below 0
 */
int contain_index(int index, int size) {
	return ((index % size) + size) % size;
}