#include <ram.h>

typedef struct {
    u8 wram[0x2000];
    u8 hram[0x80];
} ram_context;

static ram_context ctx;

u8 wram_read(u16 address) {
    address -= 0xC000;

    // Check valid address
    if (address >= 0x2000) {
        printf("INVALID WRAM ADDRESS %08X\n", address + 0xC000);
        exit(-1);
    }

    return ctx.wram[address];
}

void wram_write(u16 address, u8 value) {
    address -= 0xC000;

    // Check valid address
    if (address >= 0x2000) {
        printf("INVALID WRAM ADDRESS %08X\n", address + 0xC000);
        exit(-1);
    }

    ctx.wram[address] = value;
}

u8 hram_read(u16 address) {
    address -= 0xFF80;

    // Check valid address
    if (address >= 0x80) {
        printf("INVALID HRAM ADDRESS %08X\n", address + 0xFF80);
        exit(-1);
    }

    return ctx.hram[address];
}

void hram_write(u16 address, u8 value) {
    address -= 0xFF80;

    // Check valid address
    if (address >= 0x80) {
        printf("INVALID HRAM ADDRESS %08X\n", address + 0xFF80);
        exit(-1);
    }

    ctx.hram[address] = value;
}