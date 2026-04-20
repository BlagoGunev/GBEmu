#include <ppu.h>
#include <lcd.h>

void pixel_fifo_push(u32 value) {
    fifo_node *next = malloc(sizeof(fifo_node));
    if (next == NULL) {
        exit(-1);
    }
    next->next = NULL;
    next->value = value;
    ppu_context *ctx = ppu_get_context();
    if (!ctx->pfc.pixel_fifo.head) {
        ctx->pfc.pixel_fifo.tail = next;
        ctx->pfc.pixel_fifo.head = next;
        ctx->pfc.pixel_fifo.size = 1;
        return;
    } else {
        ctx->pfc.pixel_fifo.tail->next = next;
        ctx->pfc.pixel_fifo.tail = next;
        ctx->pfc.pixel_fifo.size++;
    }
}

u32 pixel_fifo_pop() {
    ppu_context *ctx = ppu_get_context();
    if (ctx->pfc.pixel_fifo.size <= 0 || !ctx->pfc.pixel_fifo.head) {
        fprintf(stderr, "Error in pixel fifo\n");
        exit(-8);
    }
    fifo_node *popped = ctx->pfc.pixel_fifo.head;
    u32 value = popped->value;
    ctx->pfc.pixel_fifo.head = popped->next;
    ctx->pfc.pixel_fifo.size--;

    free(popped);

    return value;
}

void pipeline_process() {
    ppu_get_context()->pfc.map_y = (lcd_get_context()->ly + lcd_get_context()->scroll_y);
    ppu_get_context()->pfc.map_x = (lcd_get_context()->scroll_x + ppu_get_context()->pfc.fetch_x);
}
