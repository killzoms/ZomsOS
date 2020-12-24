#include "stdio.h"
#include "utils.h"
#include "low_level.h"


int get_cursor()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2;
}

void set_cursor(int offset)
{
    offset /= 2;

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (char)((offset >> 8) & 0xFF));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (char)(offset & 0xFF));
}

int get_screen_offset(int col, int row)
{
    int cursor_offset = 0;

    cursor_offset = ((row * MAX_COLS) + col) * 2;

    return cursor_offset;
}


int handle_scrolling(int offset)
{
    if (offset < MAX_ROWS * MAX_COLS * 2)
    {
        return offset;
    }

    int i;

    for (i = 1; i < MAX_ROWS; i++)
    {
        mem_cpy((char*)get_screen_offset(0, i) + VIDEO_ADDRESS,
            (char*)get_screen_offset(0, i - 1) + VIDEO_ADDRESS,
            MAX_COLS * 2);
    }

    char* last_line = (char*)get_screen_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
    for (i = 0; i < MAX_COLS * 2; i++)
    {
        last_line[i] = 0;
    }

    offset -= 2 * MAX_COLS;

    return offset;
}

void print_char(char character, int col, int row, char attribute_byte)
{
    unsigned char* vidmem = (unsigned char*)VIDEO_ADDRESS;

    if (!attribute_byte)
    {
        attribute_byte = 0x0f;
    }

    int offset;
    if (col >= 0 && row >= 0)
    {
        offset = get_screen_offset(col, row);
    }
    else
    {
        offset = get_cursor();
    }

    if (character == '\n')
    {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    }
    else
    {
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }

    offset += 2;
    
    offset = handle_scrolling(offset);

    set_cursor(offset);
}

void clear_screen()
{
    for (int row = 0; row < MAX_ROWS; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            print_char(' ', col, row, 0x0f);
        }
    }

    set_cursor(get_screen_offset(0, 0));
}

void print_at(char* msg, int col, int row)
{
    if (col >= 0 && row >= 0)
    {
        set_cursor(get_screen_offset(col, row));
    }

    int i = 0;
    while (msg[i] != 0)
    {
        print_char(msg[i++], col, row , 0x0f);
    }
}

void printf(char* msg)
{
    print_at(msg, -1, -1);
}