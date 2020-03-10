#include "Snake.h"

#include <stdlib.h>
#include <stdio.h>

#define START_LENGTH 3

#define DIR_LEFT  0
#define DIR_UP    1
#define DIR_RIGHT 2
#define DIR_DOWN  3

#define CELL_EMPTY 0
#define CELL_SNAKE 1
#define CELL_APPLE 2

typedef struct Pos Pos;

int contain_index(int index, int size);

/* safely handle negative indices */

struct Snake {
	/* general state */
	int score;
	int width;
	int height;

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

Snake* Snake_create(int width, int height) {
	Snake* snake;
	snake = malloc(sizeof(Snake));
	if (!snake) {
		return NULL;
	}

	/* initialize struct */
	snake->score = 0;
	snake->width = width;
	snake->height = height;

	snake->direction = DIR_RIGHT;
	snake->last_direction = DIR_RIGHT;
	snake->length = START_LENGTH;
	snake->head_offset = 0;
	snake->pos_size = width * height + 1;
	snake->pos = malloc(sizeof(Pos) * snake->pos_size);
	snake->apple_pos = malloc(sizeof(Pos));
	if (!snake->pos || !snake->apple_pos) {
		return NULL;
	}

	/* place protagonist */
	int center_x = width / 2;
	int center_y = height / 2;
	for (int i = 0; i < snake->length; ++i) {
		snake->pos[snake->head_offset].x = center_x - i;
		snake->pos[snake->head_offset].y = center_y;
		++snake->head_offset;
	}

	return snake;
}

void Snake_destroy(Snake* snake) {
	printf("pos_size = %d\n", snake->pos_size);

	free(snake->pos);
	free(snake->apple_pos);
	free(snake);
}

int Snake_direction(Snake* snake, int direction) {
	if (direction < 0 || direction > 3) {
		return TURN_INVALID;
	}

	/* snake can't do 180° turns */
	if ((snake->last_direction == DIR_LEFT && direction == DIR_RIGHT)
	 || (snake->last_direction == DIR_UP && direction == DIR_DOWN) 
	 || (snake->last_direction == DIR_RIGHT && direction == DIR_LEFT)
	 || (snake->last_direction == DIR_DOWN && direction == DIR_UP)) {
		return TURN_NO_180;
	}

	/* update direction */
	snake->direction = direction;
	return TURN_OK;
}

int Snake_update(Snake* snake) {
	/* save some typing later on*/
	Pos* p = snake->pos;
	int head_offset = snake->head_offset;
	Pos head_pos = p[head_offset];

	/* move and update head position's index in buffer */
	int new_head_offset = (head_offset + 1) % snake->pos_size;
	

	switch (snake->direction) {
	case DIR_LEFT:
		p[new_head_offset].x = contain_index(p[head_offset].x - 1, snake->width);
		p[new_head_offset].y = p[head_offset].y;
		break;
	case DIR_UP:
		p[new_head_offset].x = p[head_offset].x;
		p[new_head_offset].y = contain_index(p[head_offset].y - 1, snake->height);
		break;
	case DIR_RIGHT:
		p[new_head_offset].x = contain_index(p[head_offset].x + 1, snake->width);
		p[new_head_offset].y = p[head_offset].y;
		break;
	case DIR_DOWN:
		p[new_head_offset].x = p[head_offset].x;
		p[new_head_offset].y = contain_index(p[head_offset].y + 1, snake->height);
		break;
	}
	snake->last_direction = snake->direction;

	snake->head_offset = new_head_offset;
	head_offset = new_head_offset;
	head_pos = p[new_head_offset];

	/* check for collisions */
	for (int i = 1; i < snake->length; ++i) { // TODO: optimize; minimum length for collision is 5
		int part_index = contain_index(head_offset - i, snake->pos_size);
		if (head_pos.x == p[part_index].x
		 && head_pos.y == p[part_index].y) {
			/* head's position is the same as at least one
			 * body part's position: collision and death   
			 */
			return MOVE_DEATH;
		}
	}
	return MOVE_OK;
}

/* helper function that wraps around "index" 
 * when it reaches "size" or goes below 0
 */
int contain_index(int index, int size) {
	return ((index % size) + size) % size;
}