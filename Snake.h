#ifndef SNAKE_H
#define SNAKE_H


#define UPDATE_MOVE  0
#define UPDATE_DIE   1
#define UPDATE_EAT   2
#define UPDATE_LOST  3
#define UPDATE_WON   4

#define TURN_INVALID -1
#define TURN_NO_180  0
#define TURN_OK      1

#define DIR_LEFT  0
#define DIR_UP    1
#define DIR_RIGHT 2
#define DIR_DOWN  3

typedef struct Snake Snake;

Snake* Snake_create(int width, int height);
void   Snake_destroy(Snake* snake);
int    Snake_turn(Snake* snake, int direction);
int    Snake_update(Snake* snake);

void   Snake_print_parts(Snake* s); 


#endif