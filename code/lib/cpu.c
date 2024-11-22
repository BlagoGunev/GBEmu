#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = {0};

void cpu_init() {
    ctx.regs.pc = 0x100;
}

static void fetch_instruction() {
    ctx.cur_opcode = bus_read(ctx.regs.pc);
    ctx.regs.pc++;
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);
}

void fetch_data();

static void execute() {
    INST_PROC proc = inst_get_processor(ctx.cur_inst->type);

    if (!proc) {
        NO_IMPL
    }

    proc(&ctx);
}

bool cpu_step() {
    
    if (!ctx.halted) {
        u16 pc = ctx.regs.pc;

        fetch_instruction();
        fetch_data();

        char flags[16];
        sprintf(flags, "%c%c%c%c", 
            ctx.regs.f & (1 << 7) ? 'Z' : '-',
            ctx.regs.f & (1 << 6) ? 'N' : '-',
            ctx.regs.f & (1 << 5) ? 'H' : '-',
            ctx.regs.f & (1 << 4) ? 'C' : '-'
        );

        printf("Ticks: %lld\nExecuting: %02X,   PC: %04X, Data (%02X %02X), Fetched: %04X\n\tA: %02X, F: %s, BC: %02X%02X, DE: %02X%02X, HL: %02X%02X\n", 
            emu_get_context()->ticks, ctx.cur_opcode, pc, bus_read(pc+1), bus_read(pc+2), 
            ctx.fetched_data, ctx.regs.a, flags, ctx.regs.b, ctx.regs.c, ctx.regs.d, ctx.regs.e, ctx.regs.h, ctx.regs.l);
        
        if (ctx.cur_inst == NULL) {
            printf("Unknown instruction: %02X\n", ctx.cur_opcode);
            exit(-7);
        }

        execute();
    }

    return true;
}
