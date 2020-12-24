#include "utils.h"

void mem_cpy(char* source, char* dest, int no_bytes)
{
	for (int i = 0; i < no_bytes; i++)
	{
		*(dest + i) = *(source + i);
	}
}