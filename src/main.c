#include <stdio.h>
#include <stdint.h>
#include <string.h>
// #include "memory.h"

#define BUFFERSIZE 2047


typedef uint8_t u8;
typedef uint16_t u16;

char* select(char* buffer, u16 start_index, u16 end_index) {
return 0;
}

u8 is_if_expression(char* buffer) {
	char substring[5];
	strncpy(substring, buffer, 4);
	substring[4] = '\0';
	if (!strcmp(substring, "(if ")) {
		fputs("got if\n", stdout);

	}
	return 0;
}


int main(const int argc, const char **argv)
{	
	char input_buffer[BUFFERSIZE];
	uint8_t running = 1;
	while (running) {
		fputs("lisp> ", stdout);
		fgets(input_buffer, BUFFERSIZE, stdin);
		if (strcmp(input_buffer, "(exit)\n") == 0) {running = 0;}
		
		is_if_expression(input_buffer);
	}
	fputs("exiting...\n", stdout);
	return 0;
}
