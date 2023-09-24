#include <stdio.h>
#include <math.h>

#include "ncurses.h"
#include "ram.c"
#include "rom.c"
#include "fake6502.c"

/**
 * Simplistic 6502 emulator. The aim of this project is minimal
 * compatibility with the Commodore 64, so games can be written for the
 * C64 and also ported to the PC without bundling a full emulator.
 */

uint16_t reset_addr;

void usage()
{
    printf("USAGE: engine [PROGRAM_FILE]\n");
    printf("\n");
    printf("Ensure the program is compiled into 6502 machine code. The\n");
    printf("program will be loaded into memory starting at address $4000,\n");
    printf("with the reset vector pointed there.\n");
    printf("\n");
    printf("KEYBOARD SHORTCUTS:\n");
    printf("\n");
    printf("r\treset 6502\n");
    printf("s\ttoggle step mode (like a debugger)\n");
    printf("l\ttoggle CPU limiting / slowdown\n");
    printf("q\tquit engine\n");
}

int read_into_memory(const char *filename)
{
    init_ram();

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERROR: Error reading PROGRAM_FILE from filesystem!\n");
        printf("\n");
        usage();

        return 0;
    }

    // get filesize
    fseek(f, 0L, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    if (filesize < 3) {
        printf("ERROR: PROGRAM_FILE incorrect size or format\n");
        printf("\n");
        usage();

        return 0;
    }

    // read reset address vector - 2 bytes at start of file
    fread(&reset_addr, 2, 1, f);
    printf("Reset: %X\n", reset_addr);
    // read file into our memory buffer
    uint8_t *memory_start = memory + reset_addr;
    fread(memory_start, 1, filesize - 2, f);

    fclose(f);

    return 1;
}

void sleep_ms(int milliseconds)
{
    #ifdef WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

uint8_t read6502(uint16_t address)
{
    // TODO return ROM
    uint8_t rom_data = read_rom(address);
    if (rom_data)
        return rom_data;
    else
        return read_ram(address);
}

void write6502(uint16_t address, uint8_t value)
{
    write_ram(address, value); // TODO check return value
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();

        return 1;
    }

    /* initscr(); */
    /* nodelay(stdscr, true); */
    /* curs_set(0); */

    // write the program into memory
    if (!read_into_memory(argv[1])) {
        return 1;
    }

    // set reset vector to 0x4000 & reset the cpu
    memory[0xFFFC] = reset_addr & 255;
    memory[0xFFFD] = reset_addr >> 8;
    reset6502();

    bool step = false;
    bool slowdown = false;
    while (1) {
        // debugging
        /* mvprintw(20, 0, "PC: %X\n", pc); */
        /* mvprintw(21, 0, "Current Instruction: %X\n", memory[pc]); */
        /* mvprintw(22, 0, "A: %X\n", a); */
        /* refresh(); */

        step6502();

        if (slowdown) sleep_ms(10);

        // character handling - press "s" to enter step mode (like a debugger), "r" to reset, "u" to unlimit cpu
        /* char ch = getch(); */
        /* if (ch == 'q') break; */
        /* if (ch == 'r') reset6502(); */
        /* if (ch == 'l') slowdown = !slowdown; */
        /* if (ch == 's') { */
        /*     if (step) { */
        /*         nodelay(stdscr, true); */
        /*         step = false; */
        /*     } else { */
        /*         nodelay(stdscr, false); */
        /*         step = true; */
        /*     } */
        /* } */
    }

    /* endwin(); */

    return 0;
}
