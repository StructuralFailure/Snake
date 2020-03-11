/*
 * circular buffer test
 */

#include <stdio.h>

#include "Snake.h"

#define DEF_WIDTH 10
#define DEF_HEIGHT 10

void snake_simulator(void);

int main(int argc, char** argv) {
	snake_simulator();

	return 0;
}

void snake_simulator(void) {
	Snake* snake;
	snake = Snake_create(DEF_WIDTH, DEF_HEIGHT);
	Snake_print_parts(snake);
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
			break;
		case '-':
			break;
		default:
			entered = 0;
		}
		if (entered) {
			getc(stdin);
		}

		Snake_update(snake);
		printf("parts:\n");
		Snake_print_parts(snake);

	}
	Snake_destroy(snake);
}