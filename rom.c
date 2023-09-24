/**
 * TODO WIP KERNAL ROM. The kernal will be written in std C so it is
 * cross platform and uses the native OS' file libraries. Perhaps we
 * want to support an *actual* c64 kernal ROM at some point - although
 * I'm not sure how helpful that is without disk drive emulation &
 * serial stuff. Perhaps implementing basic serial in C along with
 * rudimentary disk drive emulation is best, that way our kernal can be
 * written in custom assembly code.
 *
 * Things we need at a minimum:
 *
 *  SETLFS
 *  SETNAM
 *  SAVE
 *  LOAD
 *
 * Preferably with:
 *
 *  OPEN (supporting popular needs such as scratching & renaming files)
 *  CLOSE
 *  SCNKEY (necessary?)
 *  Reset functions (SCINIT, IOINIT, RAMTAS, etc.)
 *  CHROUT
 *  CHRIN
 *  GETIN
 *  SCREEN
 *  PLOT
 *
 * DON'T need:
 *
 *  TALK, IECIN, etc. (none of the serial stuff)
 *
 * Might need / nice to have:
 *
 *  READST
 *  SETTIM
 *  RDTIM
 *  CLRCHN
 *  CHKOUT (is this just for serial stuff?)
 *  CHKIN (is this just for serial stuff?)
 */

extern uint8_t a, x, y;
static void rts();

uint8_t  lfs_fd;
uint8_t  lfs_device;
uint8_t  lfs_cmd;
uint16_t nam_filename; // filename address
uint8_t  nam_filesize; // filename size
uint16_t save_start;   // start address to save
uint16_t save_end;     // end address to save

// handle address via ROM if the bank is switched on
// TODO should verify JSR first
// TODO need a method to verify if ROM at address so we don't rely on 0
uint8_t read_rom(uint16_t address)
{
    switch (address) {
        case 0xFFBA: // SETLFS
            lfs_fd = a;
            lfs_device = x;
            lfs_cmd = y;

            return 0x60; // RTS
        case 0xFFBD: // SETNAM
            nam_filename = (y << 8) | x;
            nam_filesize = a;

            return 0x60; // RTS
        case 0xFFD8: // SAVE
            save_start = a;
            save_end = (y << 8) | x;
            do_save();

            return 0x60; // RTS
    }

    return 0;
}

uint8_t read_ram(uint16_t address);

// perform the save operation via c stdlib
void do_save()
{
    FILE *fd;
    uint16_t cur_address;
    char *filename;
    char *cur_filename;
    uint8_t data_to_write;

    filename = malloc(sizeof(uint8_t) * nam_filesize + 1);

    if (filename == NULL) {
        // TODO panic
        printf("KERNAL ERROR: Out of memory\n");
        return;
    }

    for (uint8_t i=0; i < nam_filesize; i++) {
        filename[i] = read_ram(nam_filename + i);
    }
    filename[nam_filesize] = '\0';

    printf("FILENAME: %s\n", filename);

    // TODO should have multiple pointers when we support OPEN & CLOSE
    fd = fopen(filename, "wb");

    if (fd == NULL) {
        // TODO panic
        printf("KERNAL ERROR: Unable to open file for writing\n");
        return;
    }

    // expand pointer at zeropage to save start
    cur_address = (read_ram(save_start + 1) << 8) | read_ram(save_start);
    printf("CUR ADDR: %X\n", cur_address);
    // write the reset vector first
    data_to_write = cur_address & 0xFF;
    fwrite(&data_to_write, 1, sizeof data_to_write, fd);
    data_to_write = cur_address >> 8;
    fwrite(&data_to_write, 1, sizeof data_to_write, fd);
    // write data from cur_address to save_end
    while (cur_address < save_end) {
        data_to_write = read_ram(cur_address);
        fwrite(&data_to_write, 1, sizeof data_to_write, fd);
        cur_address ++;
    }

    fclose(fd);
}
