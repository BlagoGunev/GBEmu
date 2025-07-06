#pragma once

#include <common.h>
#include <instructions.h>

// AF	A	-	Accumulator & Flags
// BC	B	C	BC
// DE	D	E	DE
// HL	H	L	HL
// SP	-	-	Stack Pointer
// PC	-	-	Program Counter/Pointer

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
} cpu_registers;

typedef struct {
    cpu_registers regs;

    u16 fetched_data;
    u16 mem_dest;
    bool dest_is_mem;
    u8 cur_opcode;
    instruction *cur_inst;

    bool halted;
    bool stepping;

    bool int_master_enabled;
    bool enable_ime;
    u8 ie_register;
    u8 int_flags;
    
} cpu_context;

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_N BIT(ctx->regs.f, 6)
#define CPU_FLAG_H BIT(ctx->regs.f, 5)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

void cpu_init();
bool cpu_step();

cpu_registers *cpu_get_regs();

u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 val);
u8 cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, u8 val);
void cpu_set_flags(cpu_context *ctx, int8_t z, int8_t n, int8_t h, int8_t c);

u8 cpu_get_ie_register();
void cpu_set_ie_register(u8 n);

u8 cpu_get_int_flags();
void cpu_set_int_flags(u8 value);

typedef void (* INST_PROC)(cpu_context *);
INST_PROC inst_get_processor(inst_type type);
