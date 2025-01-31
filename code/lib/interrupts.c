#include <interrupts.h>
#include <cpu.h>
#include <bus.h>
#include <stack.h>

void int_jump_address(cpu_context *ctx, u16 address) {
    stack_push16(ctx->regs.pc);
    ctx->regs.pc = address;
}

bool int_try_handle(cpu_context *ctx, u16 address, interrupt_type t) {
    if (ctx->int_flags & t && ctx->ie_register & t) {
        int_jump_address(ctx, address);
        ctx->ie_register &= ~t;
        ctx->halted = false;
        ctx->int_master_enabled = false;
    }
}

void cpu_request_interrupt(interrupt_type t) {

}

void cpu_handle_interrupts(cpu_context *ctx) {
    if (int_try_handle(ctx, 0x40, IT_VBLANK)) return;
    if (int_try_handle(ctx, 0x48, IT_LCD_STAT)) return;
    if (int_try_handle(ctx, 0x50, IT_TIMER)) return;
    if (int_try_handle(ctx, 0x58, IT_SERIAL)) return;
    if (int_try_handle(ctx, 0x60, IT_JOYPAD)) return;
}