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
    u16 sp;
    u16 pc;
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
    
} cpu_context;

void cpu_init();
bool cpu_step();

u16 cpu_read_reg(reg_type rt);
