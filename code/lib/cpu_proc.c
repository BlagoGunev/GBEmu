#include <cpu.h>
#include <bus.h>
#include <emu.h>
#include <stack.h>

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

static bool is_16_bit_reg(reg_type rt) {
    return rt >= RT_AF;
}

reg_type reg_lookup[] = {
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_HL,
    RT_A
};

reg_type decode_reg(u8 reg) {
    if (reg > 0x7) {
        return RT_NONE;
    }

    return reg_lookup[reg];
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
        if (is_16_bit_reg(ctx->cur_inst->reg_2)) {
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
        cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_2) + (int8_t)ctx->fetched_data);
        
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

static void goto_addr(cpu_context *ctx, u16 addr, bool pushpc) {
    if (check_cond(ctx)) {
        if (pushpc) {
            emu_cycles(2);
            stack_push16(ctx->regs.pc);
        }

        ctx->regs.pc = addr;
        emu_cycles(1);
    }
}

static void proc_jp(cpu_context *ctx) {
    goto_addr(ctx, ctx->fetched_data, false);
}

static void proc_jr(cpu_context *ctx) {
    int8_t rel = (int8_t)(ctx->fetched_data & 0xFF);
    u16 addr = ctx->regs.pc + rel;
    goto_addr(ctx, addr, false);
}

static void proc_call(cpu_context *ctx) {
    goto_addr(ctx, ctx->fetched_data, true);
}

static void proc_ret(cpu_context *ctx) {
    if (ctx->cur_inst->cond != CT_NONE) {
        emu_cycles(1);
    }

    if (check_cond(ctx)) {
        u16 lo = stack_pop();
        emu_cycles(1);
        u16 hi = stack_pop();
        emu_cycles(1);

        u16 n = (hi << 8) | lo;
        ctx->regs.pc = n;

        emu_cycles(1);
    }
}

static void proc_reti(cpu_context *ctx) {
    ctx->int_master_enabled = true;
    proc_ret(ctx);
}

static void proc_rst(cpu_context *ctx) {
    goto_addr(ctx, ctx->cur_inst->param, true);
}

static void proc_cb(cpu_context *ctx) {
    u8 op = ctx->fetched_data;
    reg_type reg = decode_reg(op & 0x7);
    u8 reg_val = cpu_read_reg8(reg);
    u8 bit = (op >> 3) & 0x3;
    u8 bit_op = (op >> 6) & 0x3;

    emu_cycles(1);

    if (reg == RT_HL) {
        emu_cycles(2);
    }

    switch(bit_op) {
        case 1:
            // BIT
            cpu_set_flags(ctx, !BIT(reg_val, 0), 0, 1, -1);
            return;
        case 2: 
            // RST
            reg_val &= ~(1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
        case 3:
            // SET
            reg_val |= (1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
    }

    bool flagC = CPU_FLAG_C;

    switch (bit) {
        case 0: {
            // RLC
            bool setC = BIT(reg_val, 7);
            u8 result = ((reg_val << 1) & 0xFF) | setC;
            cpu_set_reg8(reg, result);
            cpu_set_flags(ctx, result == 0, 0, 0, setC);
        } return;

        case 1: {
            // RRC
            u8 old = reg_val;
            reg_val >>= 1;
            reg_val |= (old << 7);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, reg_val == 0, 0, 0, old & 1);
        } return;

        case 2: {
            // RL
            u8 old = reg_val;
            reg_val <<= 1;
            reg_val |= flagC;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, reg_val == 0, 0, 0, !!(old & 0x80));
        } return;

        case 3: {
            // RR
            u8 old = reg_val;
            reg_val >>= 1;
            reg_val |= (flagC << 7);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, reg_val == 0, 0, 0, old & 1);
        } return;

        case 4: {
            // SLA
            u8 old = reg_val;
            reg_val <<= 1;
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, reg_val == 0, 0, 0, !!(old & 0x80));
        } return;

        case 5: {
            // SRA
            u8 u = (int8_t)reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags(ctx, u == 0, 0, 0, reg_val & 1);
        } return;

        case 6: {
            // SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, reg_val == 0, 0, 0, 0);
        } return;

        case 7: {
            // SRL
            u8 u = reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags(ctx, u == 0, 0, 0, reg_val & 1);
        } return;
    }

    fprintf(stderr, "ERROR INCORRECT CB: %02X", op);
    NO_IMPL
}

static void proc_pop(cpu_context *ctx) {
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);

    u16 n = (hi << 8) | lo;

    cpu_set_reg(ctx->cur_inst->reg_1, n);
    
    // TODO: check flags correctness
    if (ctx->cur_inst->reg_1 == RT_AF) {
        cpu_set_reg(ctx->cur_inst->reg_1, n & 0xFFF0);
    }
}

static void proc_push(cpu_context *ctx) {
    u16 hi = (cpu_read_reg(ctx->cur_inst->reg_1) >> 8) & 0xFF;
    emu_cycles(1);
    stack_push(hi);

    u16 lo = cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF;
    emu_cycles(1);
    stack_push(lo);

    emu_cycles(1);
}

static void proc_rlca(cpu_context *ctx) {
    u8 u = ctx->regs.a;
    bool c = (u >> 7) & 1;
    u = (u << 1) | c;

    ctx->regs.a = u;
    cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_rrca(cpu_context *ctx) {
    u8 u = ctx->regs.a;
    bool c = u & 1;
    u = (u >> 1) | (c << 7);

    ctx->regs.a = u;
    cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_rla(cpu_context *ctx) {
    u8 u = ctx->regs.a;
    u8 cf = CPU_FLAG_C;
    u8 c = (u >> 7) & 1;

    ctx->regs.a = (u << 1) | cf;
    cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_rra(cpu_context *ctx) {
    u8 u = ctx->regs.a;
    u8 cf = CPU_FLAG_C;
    u8 c = u & 1;

    ctx->regs.a = (u >> 1) | (cf << 7);
    cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_di(cpu_context *ctx) {
    ctx->int_master_enabled = false;
}

static void proc_and(cpu_context *ctx) {
    ctx->regs.a &= ctx->fetched_data;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 1, 0);
}

static void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

static void proc_or(cpu_context *ctx) {
    ctx->regs.a |= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

static void proc_cp(cpu_context *ctx) {
    int n = (int)ctx->regs.a - (int)ctx->fetched_data;

    cpu_set_flags(ctx, n == 0, 1, ((int)ctx->regs.a & 0x0F) - ((int)ctx->fetched_data & 0x0F) < 0, n < 0);
}

static void proc_inc(cpu_context *ctx) {
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) + 1;

    if (is_16_bit_reg(ctx->cur_inst->reg_1)) {
        emu_cycles(1);
    }

    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR) {
        val = bus_read(cpu_read_reg(RT_HL)) + 1;
        val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    } else {
        cpu_set_reg(ctx->cur_inst->reg_1, val);
        val = cpu_read_reg(ctx->cur_inst->reg_1);
    }

    if ((ctx->cur_opcode & 0x03) == 0x03) {
        return;
    }

    cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}

static void proc_dec(cpu_context *ctx) {
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) - 1;

    if (is_16_bit_reg(ctx->cur_inst->reg_1)) {
        emu_cycles(1);
    }

    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR) {
        val = bus_read(cpu_read_reg(RT_HL)) - 1;
        bus_write(cpu_read_reg(RT_HL), val);
    } else {
        cpu_set_reg(ctx->cur_inst->reg_1, val);
        val = cpu_read_reg(ctx->cur_inst->reg_1);
    }

    if ((ctx->cur_opcode & 0x0B) == 0x0B) {
        return;
    }

    cpu_set_flags(ctx, val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

static void proc_adc(cpu_context *ctx) {
    u16 u = ctx->fetched_data;
    u16 a = ctx->regs.a;
    u16 c = CPU_FLAG_C;

    ctx->regs.a = (u + a + c) & 0xFF;

    cpu_set_flags(ctx, ctx->regs.a == 0, 0, (a & 0xF) + (u & 0xF) + c > 0xF, a + u + c > 0xFF);
}

static void proc_add(cpu_context *ctx) {
    u32 val = cpu_read_reg(ctx->cur_inst->reg_1) + ctx->fetched_data;

    bool is_16_bit = is_16_bit_reg(ctx->cur_inst->reg_1);

    if (is_16_bit) {
        emu_cycles(1);
    }

    if (ctx->cur_inst->reg_1 == RT_SP) {
        val = cpu_read_reg(RT_SP) + (char)ctx->fetched_data;
    }

    int z = (val & 0xFF) == 0;
    int h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
    int c = (int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF) + (int)(ctx->fetched_data & 0xFF) >= 0x100;

    if (is_16_bit) {
        z = -1;
        h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xFFF) + (ctx->fetched_data & 0xFFF) >= 0x1000;
        u32 n = (u32)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xFFFF) + (u32)(ctx->fetched_data & 0xFFFF);
        c = n >= 0x10000;
    }

    if (ctx->cur_inst->reg_1 == RT_SP) {
        z = 0;
        h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
        c = (int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF) + (int)(ctx->fetched_data & 0xFF) >= 0x100;
    }

    cpu_set_reg(ctx->cur_inst->reg_1, val & 0xFFFF);
    cpu_set_flags(ctx, z, 0, h, c);
}

static void proc_sub(cpu_context *ctx) {
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) - ctx->fetched_data;

    int z = val == 0;
    int h = ((int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xF)) - ((int)(ctx->fetched_data & 0xF)) < 0;
    int c = ((int)cpu_read_reg(ctx->cur_inst->reg_1)) - ((int)ctx->fetched_data) < 0;

    cpu_set_reg(ctx->cur_inst->reg_1, val);
    cpu_set_flags(ctx, z, 1, h, c); 
}

static void proc_sbc(cpu_context *ctx) {
    u8 val = ctx->fetched_data + CPU_FLAG_C;

    int z = cpu_read_reg(ctx->cur_inst->reg_1) - val == 0;
    int h = ((int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xF)) 
        - ((int)(ctx->fetched_data & 0xF)) - ((int)CPU_FLAG_C) < 0;
    int c = ((int)cpu_read_reg(ctx->cur_inst->reg_1)) 
        - ((int)ctx->fetched_data) - ((int)CPU_FLAG_C) < 0;

    cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_1) - val);
    cpu_set_flags(ctx, z, 1, h, c); 
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
    [INST_JR] = proc_jr,
    [INST_CALL] = proc_call,
    [INST_RET] = proc_ret,
    [INST_RETI] = proc_reti,
    [INST_RST] = proc_rst,
    [INST_CB] = proc_cb,
    [INST_POP] = proc_pop,
    [INST_PUSH] = proc_push,
    [INST_RLCA] = proc_rlca,
    [INST_RRCA] = proc_rrca,
    [INST_RLA] = proc_rla,
    [INST_RRA] = proc_rra,
    [INST_DI] = proc_di,
    [INST_AND] = proc_and,
    [INST_XOR] = proc_xor,
    [INST_OR] = proc_or,
    [INST_CP] = proc_cp,
    [INST_INC] = proc_inc,
    [INST_DEC] = proc_dec,
    [INST_ADC] = proc_adc,
    [INST_ADD] = proc_add,
    [INST_SUB] = proc_sub,
    [INST_SBC] = proc_sbc
};

INST_PROC inst_get_processor(inst_type type) {
    return processors[type];
}