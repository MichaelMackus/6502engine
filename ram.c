#include "ncurses.h"

// 64k of memory! Who would ever use that much!?!
#define MEMORY_SIZE 64*1024
#define SCREEN_COLS 40
#define SCREEN_ROWS 25

uint8_t memory[MEMORY_SIZE];
uint8_t screen_buffer[SCREEN_COLS * SCREEN_ROWS];
uint8_t color_buffer[SCREEN_COLS * SCREEN_ROWS];

void update_screen(uint16_t offset)
{
    if (offset > SCREEN_COLS * SCREEN_ROWS) {
        // TODO error/panic function
        return;
    }

    // get row and column based on address
    uint8_t row = floor(((double) offset / (double) SCREEN_COLS));
    uint8_t col = offset - row*SCREEN_COLS;
    uint8_t color = color_buffer[offset];

    attron(COLOR_PAIR(color));

    // print using curses
    mvaddch(row, col, screen_buffer[offset]);
}

uint8_t screen_code_to_ascii(uint8_t screen_code)
{
    if (screen_code < 27) {
        return screen_code + 64;
    }

    return screen_code;
}

int init_ram()
{
    for (int i=0; i<MEMORY_SIZE; i++) {
        memory[i] = 0;
    }


    if (has_colors() == FALSE) {
        printf("Your terminal does not support color\n");

        return 0;
    }


    start_color();
    init_pair(0, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // violet
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
    init_pair(8, COLOR_YELLOW, COLOR_BLACK); // orange
    init_pair(9, COLOR_RED, COLOR_BLACK); // brown
    init_pair(10, COLOR_RED, COLOR_BLACK); // light red
    init_pair(11, COLOR_BLACK, COLOR_BLACK); // dark grey
    init_pair(12, COLOR_BLACK, COLOR_BLACK); // grey
    init_pair(13, COLOR_GREEN, COLOR_BLACK); // light green
    init_pair(14, COLOR_BLUE, COLOR_BLACK); // light blue
    init_pair(15, COLOR_WHITE, COLOR_BLACK); // light grey

    return 1;
}

uint8_t read_ram(uint16_t address)
{
    return memory[address];
}

int write_ram(uint16_t address, uint8_t value)
{
    /* if (address & 0xD000) */

    // screen RAM 40 column mode
    if (address >= 0x400 && address <= 0x7E7) {
        screen_buffer[address - 0x400] = screen_code_to_ascii(value);
        update_screen(address - 0x400);
    }

    // color RAM
    if (address >= 0xD800 && address <= 0xDBE7) {
        color_buffer[address - 0xD800] = value;
        update_screen(address - 0xD800);
    }

    // TODO 0xD021 bg color
    // TODO 0xD020 border color
    // TODO 0x0286 cursor color

    memory[address] = value;

    return 1;
}
