/*
 * circular buffer test
 */

#include <stdio.h>

#include "Snake.h"

#define DEF_WIDTH 10
#define DEF_HEIGHT 10

int main(int argc, char** argv) {
	int test = -2;
	printf("%d\n", test % 16);

	int size = 16;

	while (1) {
		int index;
		scanf("%d", &index);

		if (index >= 0) {
			index %= size;
		} else {
			index = ((index % size) + size) % size;
		}
		printf("%d\n", index);
	}

	Snake* snake;
	snake = Snake_create(DEF_WIDTH, DEF_HEIGHT);
	Snake_destroy(snake);
	return 0;
}