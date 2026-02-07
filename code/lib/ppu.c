#include <ppu.h>
#include <ppu_sm.h>
#include <lcd.h>
#include <string.h>

static ppu_context ctx;

void ppu_init() {
    ctx.current_frame = 0;
    ctx.line_ticks = 0;
    ctx.video_buffer = malloc(YRES * XRES * sizeof(u32));

    lcd_init();
    LCDSTAT_MODE_SET(MODE_OAM);

    memset(ctx.oam_ram, 0, sizeof(ctx.oam_ram));
    memset(ctx.video_buffer, 0, YRES * XRES * sizeof(u32));
}

void ppu_tick() {
    ctx.line_ticks++;

    switch(LCDSTAT_MODE) {
        case MODE_OAM:
            ppu_oam();
            break;
        case MODE_XFER:
            ppu_xfer();
            break;
        case MODE_HBLANK:
            ppu_hblank();
            break;
        case MODE_VBLANK:
            ppu_vblank();
            break;
    }
}

void ppu_oam_write(u16 address, u8 value) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    u8 *p = (u8 *)ctx.oam_ram;
    p[address] = value;
}

u8 ppu_oam_read(u16 address) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    u8 *p = (u8 *)ctx.oam_ram;
    return p[address];
}

void ppu_vram_write(u16 address, u8 value) {
    ctx.vram[address - 0x8000] = value;
}

u8 ppu_vram_read(u16 address) {
    return ctx.vram[address - 0x8000];
}

ppu_context *ppu_get_context() {
    return &ctx;
}