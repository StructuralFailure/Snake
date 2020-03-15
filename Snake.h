#ifndef SNAKE_H
#define SNAKE_H

typedef enum Update {
	UPDATE_MOVE = 0,
	UPDATE_DIE,
	UPDATE_EAT,
	UPDATE_LOST,
	UPDATE_WON,
} Update;

typedef enum Turn {
	TURN_INVALID = 0,
	TURN_NO_180,
	TURN_OK,
} Turn;

typedef enum Dir {
	DIR_LEFT = 0,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
} Dir;

typedef struct Snake Snake;
typedef struct Pos {
	int x;
	int y;
} Pos;


Snake* Snake_create(int width, int height);
void   Snake_destroy(Snake* snake);
Turn   Snake_turn(Snake* snake, Dir Dir);
Update Snake_update(Snake* snake);
int    Snake_length(Snake* snake);
/* 0 <= position < length */
Pos    Snake_position(Snake* snake, int index); 

void   Snake_set_length(Snake* s, int length);


#endif