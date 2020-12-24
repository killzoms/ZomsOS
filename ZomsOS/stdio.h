#pragma once
#define VIDEO_ADDRESS 0xb8000
#define MAX_COLS 80
#define MAX_ROWS 25
#define WHITE_ON_BLACK 0x0f
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define LOOP 0

void printf(char* msg);
void print_char(char character, int col, int row, char attribute_byte);