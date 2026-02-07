#include <ppu.h>
#include <ppu_sm.h>
#include <lcd.h>
#include <interrupts.h>

void increment_ly() {
    lcd_get_context()->ly++;

    if (lcd_get_context()->ly == lcd_get_context()->ly_compare) {
        LCDSTAT_LYC_SET(1);

        if (LCDSTAT_STAT_INT(SS_LYC)) {
            cpu_request_interrupt(IT_LCD_STAT);
        }
    } else {
        LCDSTAT_LYC_SET(0);
    }
}

void ppu_oam() {
    if (ppu_get_context()->line_ticks >= 80) {
        LCDSTAT_MODE_SET(MODE_XFER);
    }
}

void ppu_xfer() {
    if (ppu_get_context()->line_ticks >= 80 + 172) {
        LCDSTAT_MODE_SET(MODE_HBLANK);
    }
}

static double target_frame_time = 1000.0 / 60.0; // ms / FPS
static u32 prev_frame_time = 0;
static u32 start_timer = 0;
static u32 frame_count = 0;

void ppu_hblank() {
    if (ppu_get_context()->line_ticks >= TICKS_PER_LINE) {
        increment_ly();

        if (lcd_get_context()->ly >= YRES) {
            LCDSTAT_MODE_SET(MODE_VBLANK);

            cpu_request_interrupt(IT_VBLANK);

            if (LCDSTAT_STAT_INT(SS_VBLANK)) {
                cpu_request_interrupt(IT_LCD_STAT);
            }

            ppu_get_context()->current_frame++;

            // FPS
            u32 end_timer = get_ticks();
            u32 frame_time = end_timer - prev_frame_time;

            if (frame_time < target_frame_time) {
                delay(target_frame_time - frame_time);
            }

            if (end_timer - start_timer >= 1000) {
                u32 fps = frame_count;
                start_timer = end_timer;
                frame_count = 0;

                printf("FPS %d\n", fps);
            }

            frame_count++;
            prev_frame_time = get_ticks();

        } else {
            LCDSTAT_MODE_SET(MODE_OAM);
        }

        ppu_get_context()->line_ticks = 0;
    }
}

void ppu_vblank() {
    if (ppu_get_context()->line_ticks >= TICKS_PER_LINE) {
        increment_ly();

        if (lcd_get_context()->ly >= LINES_PER_FRAME) {
            LCDSTAT_MODE_SET(MODE_OAM);
            lcd_get_context()->ly = 0;
        }

        ppu_get_context()->line_ticks = 0;
    }
}