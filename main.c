/*
 * circular buffer test
 */

#include <stdio.h>

#include "Snake.h"

#define DEF_WIDTH 10
#define DEF_HEIGHT 10

void game_loop(void);

int main(int argc, char** argv) {
	game_loop();

	return 0;
}

void game_loop(void) {
	Snake* snake = Snake_create(DEF_WIDTH, DEF_HEIGHT);
	char in;

	while (1) {
		in = getc(stdin);
		int entered = 1;
		switch (in) {
		case 'l':
			Snake_turn(snake, DIR_LEFT);
			break;
		case 'u':
			Snake_turn(snake, DIR_UP);
			break;
		case 'r':
			Snake_turn(snake, DIR_RIGHT);
			break;
		case 'd':
			Snake_turn(snake, DIR_DOWN);
			break;
		case '+':
			Snake_set_length(snake, Snake_length(snake) + 1);
			break;
		case '-':
			Snake_set_length(snake, Snake_length(snake) - 1);
			break;
		default:
			entered = 0;
		}
		if (entered) {
			getc(stdin);
		}

		switch (Snake_update(snake)) {
		case UPDATE_MOVE:
			printf("moved. ");
			break;
		case UPDATE_DIE:
			printf("died. ");
			break;
		case UPDATE_EAT:
			printf("ate. ");
			break;
		case UPDATE_LOST:
			printf("you're dead. ");
			break;
		case UPDATE_WON:
			printf("you've won. ");
			break;
		}

		
		int length = Snake_length(snake);
		printf("\nlength: %d\n", length);

		for (int i = 0; i < length; ++i) {
			Pos pos = Snake_position(snake, i);
			printf("[%d; %d]\n", pos.x, pos.y);
		}
		printf("\n");

	}
	Snake_destroy(snake);
}