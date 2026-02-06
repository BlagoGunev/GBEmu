#include <common.h>
#include <lcd.h>
#include <ppu.h>
#include <dma.h>

static lcd_context ctx;

static unsigned long colors_default[4] = {
    0xFFFFFFFF, // black
    0xFFAAAAAA, // dark gray
    0xFF555555, // light gray
    0xFF000000  // white
};

void lcd_init() {
    ctx.ctrl = 0x91;
    ctx.scroll_y = 0;
    ctx.scroll_x = 0;
    ctx.ly = 0;
    ctx.ly_compare = 0;
    ctx.bg_palette = 0xFC;
    ctx.obj_palette[0] = 0xFF;
    ctx.obj_palette[1] = 0xFF;
    ctx.window_y = 0;
    ctx.window_x = 0;

    for (int i = 0; i < 4; i++) {
        ctx.bg_colors[i] = colors_default[i];
        ctx.sp1_colors[i] = colors_default[i];
        ctx.sp2_colors[i] = colors_default[i];
    }
}

lcd_context *lcd_get_context() {
    return &ctx;
}

u8 lcd_read(u16 address) {
    u8 offset = address - 0xFF40;
    u8 *ptr = (u8 *)&ctx.ctrl;

    return ptr[offset];
}

void update_palette(u8 palette_data, u8 palette) {
    u32 *colors = ctx.bg_colors;

    switch (palette) {
        case 1:
            colors = ctx.sp1_colors;
            break;
        case 2:
            colors = ctx.sp2_colors;
            break;
    }

    colors[0] = colors_default[palette_data & 0b11];
    colors[1] = colors_default[(palette_data >> 2) & 0b11];
    colors[2] = colors_default[(palette_data >> 4) & 0b11];
    colors[3] = colors_default[(palette_data >> 6) & 0b11];
}

void lcd_write(u16 address, u8 value) {
    u8 offset = address - 0xFF40;
    u8 *ptr = (u8 *)&ctx.ctrl;
    ptr[offset] = value;

    // DMA
    if (offset == 6) {
        dma_start(value);
    } 
    
    // Palette changes
    if (offset == 7) {
        update_palette(value, 0);
    } else if (offset == 8) {
        update_palette(value & 0b11111100, 1);
    } else if (offset == 9) {
        update_palette(value & 0b11111100, 2);
    }
}