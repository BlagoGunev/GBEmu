#include <cpu.h>
#include <bus.h>
#include <emu.h>

static bool check_cond(cpu_context *ctx) {
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch (ctx->cur_inst->cond) {
        case CT_NONE: return true;
        case CT_C: return c;
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;

        default: return false;
    }
    return false;
}

static void proc_none(cpu_context *ctx) {
    printf("Invalid instruction! Exiting...\n");
    exit(-7);
}

static void proc_nop(cpu_context *ctx) {

}

static void proc_ld(cpu_context *ctx) {

    if (ctx->dest_is_mem) {
        
        // if 16 bit register
        if (ctx->cur_inst->reg_2 >= RT_AF) {
            bus_write16(ctx->mem_dest, ctx->fetched_data);
            emu_cycles(1);
        } else {
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }

        return;
    }

    if (ctx->cur_inst->mode == AM_HL_SPR) {
        u8 hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xF) + 
            (ctx->fetched_data & 0xF) >= 0x10;
        u8 cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF) + 
            (ctx->fetched_data & 0xFF) >= 0x100;
        
        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_2) + (char)ctx->fetched_data);
        
        return;
    }

    cpu_set_reg(ctx->cur_inst->reg_1, ctx->fetched_data);
}

static void proc_ldh(cpu_context *ctx) {
    if (ctx->cur_inst->reg_1 == RT_A) {
        cpu_set_reg(RT_A, bus_read(0xFF00 | ctx->fetched_data));
    } else {
        bus_write(ctx->mem_dest, ctx->regs.a);
    }

    emu_cycles(1);
}

static void proc_jp(cpu_context *ctx) {
    if (check_cond(ctx)) {
        ctx->regs.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}

static void proc_di(cpu_context *ctx) {
    ctx->int_master_enabled = false;
}

static void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a, 0, 0, 0);
}

void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c) {
    if (z != -1) {
        BIT_SET(ctx->regs.f, 7, z);
    }

    if (n != -1) {
        BIT_SET(ctx->regs.f, 6, n);
    }

    if (h != -1) {
        BIT_SET(ctx->regs.f, 5, h);
    }

    if (c != -1) {
        BIT_SET(ctx->regs.f, 4, c);
    }
}

static INST_PROC processors[] = {
    [INST_NONE] = proc_none,
    [INST_NOP] = proc_nop,
    [INST_LD] = proc_ld,
    [INST_LDH] = proc_ldh,
    [INST_JP] = proc_jp,
    [INST_DI] = proc_di,
    [INST_XOR] = proc_xor
};

INST_PROC inst_get_processor(inst_type type) {
    return processors[type];
}