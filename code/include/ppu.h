#pragma once

#include <common.h>

/*
    Bit 7: Priority: 0 = No, 1 = BG and Window color indices 1–3 are drawn over this OBJ
    Bit 6: Y flip: 0 = Normal, 1 = Entire OBJ is vertically mirrored
    Bit 5: X flip: 0 = Normal, 1 = Entire OBJ is horizontally mirrored
    Bit 4: DMG palette [Non CGB Mode only]: 0 = OBP0, 1 = OBP1
    Bit 3: Bank [CGB Mode Only]: 0 = Fetch tile from VRAM bank 0, 1 = Fetch tile from VRAM bank 1
    Bits 2-0: CGB palette [CGB Mode Only]: Which of OBP0–7 to use
*/
typedef struct {
    u8 y;
    u8 x;
    u8 tile;

    unsigned f_cgb_palette : 3;
    unsigned f_cgb_bank : 1;
    unsigned f_palette : 1;
    unsigned f_x_flip : 1;
    unsigned f_y_flip : 1;
    unsigned f_bg_priority : 1;
} oam_entry;

typedef struct {
    oam_entry oam_ram[40];
    u8 vram[0x2000];
} ppu_context;

void ppu_init();
void ppu_tick();

void ppu_oam_write(u16 address, u8 value);
u8 ppu_oam_read(u16 address);

void ppu_vram_write(u16 address, u8 value);
u8 ppu_vram_read(u16 address);