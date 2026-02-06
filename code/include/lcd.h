#pragma once

#include <common.h>

typedef struct {
    // Registers
    u8 ctrl;       // 0xFF40
    u8 stat;       // 0xFF41
    u8 scroll_y;   // 0xFF42
    u8 scroll_x;   // 0xFF43
    u8 ly;         // 0xFF44
    u8 ly_compare; // 0xFF45
    u8 dma;        // 0xFF46
    u8 bg_palette; // 0xFF47
    u8 obj_palette[2]; // 0xFF48-0xFF49
    u8 window_y;   // 0xFF4A
    u8 window_x;   // 0xFF4B

    // Other data
    u32 bg_colors[4];
    u32 sp1_colors[4];
    u32 sp2_colors[4];

} lcd_context;

typedef enum {
    MODE_HBLANK, // MODE 0
    MODE_VBLANK, // MODE 1
    MODE_OAM,    // MODE 2
    MODE_XFER    // MODE 3
} lcd_mode;

lcd_context *lcd_get_context();

/*
  7  LCD & PPU enable: 0 = Off; 1 = On
  6  Window tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
  5  Window enable: 0 = Off; 1 = On
  4  BG & Window tile data area: 0 = 8800–97FF; 1 = 8000–8FFF
  3  BG tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
  2  OBJ size: 0 = 8×8; 1 = 8×16
  1  OBJ enable: 0 = Off; 1 = On
  0  BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 = On
*/

#define LCDC_LCD_ENABLE (BIT(lcd_get_context()->ctrl, 7))
#define LCDC_WIN_MAP_AREA (BIT(lcd_get_context()->ctrl, 6) ? 0x9C00 : 0x9800)
#define LCDC_WIN_ENABLE (BIT(lcd_get_context()->ctrl, 5))
#define LCDC_BGW_DATA_AREA (BIT(lcd_get_context()->ctrl, 4) ? 0x8000 : 0x8800)
#define LCDC_BG_MAP_AREA (BIT(lcd_get_context()->ctrl, 3) ? 0x9C00 : 0x9800)
#define LCDC_OBJ_SIZE (BIT(lcd_get_context()->ctrl, 2) ? 16 : 8)
#define LCDC_OBJ_ENABLE (BIT(lcd_get_context()->ctrl, 1))
#define LCDC_BGW_ENABLE (BIT(lcd_get_context()->ctrl, 0))

/*
  6    LYC int select (Read/Write): If set, selects the LYC == LY condition for the STAT interrupt.
  5    Mode 2 int select (Read/Write): If set, selects the Mode 2 condition for the STAT interrupt.
  4    Mode 1 int select (Read/Write): If set, selects the Mode 1 condition for the STAT interrupt.
  3    Mode 0 int select (Read/Write): If set, selects the Mode 0 condition for the STAT interrupt.
  2    LYC == LY (Read-only): Set when LY contains the same value as LYC; it is constantly updated.
  1-0  PPU mode (Read-only): Indicates the PPU’s current status. Reports 0 instead when the PPU is disabled.
*/

typedef enum {
    SS_HBLANK = (1 << 3),
    SS_VBLANK = (1 << 4),
    SS_OAM = (1 << 5),
    SS_LYC = (1 << 6)
} lcdstat_src;

#define LCDSTAT_STAT_INT(src) (lcd_get_context()->stat & src)
#define LCDSTAT_LYC (BIT(lcd_get_context()->stat, 2))
#define LCDSTAT_LYC_SET(b) (BIT_SET(lcd_get_context()->stat, 2, b))
#define LCDSTAT_MODE ((lcd_mode)(lcd_get_context()->stat & 0b11))
#define LCDSTAT_MODE_SET(mode) {lcd_get_context()->stat &= ~0b11; lcd_get_context()->stat |= mode; }

void lcd_init();

u8 lcd_read(u16 address);
void lcd_write(u16 address, u8 value);