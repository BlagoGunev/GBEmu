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

static void fetch_data() {
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    if (ctx.cur_inst == NULL) {
        return;
    }

    switch(ctx.cur_inst->mode) {
        case AM_IMP: return;

        case AM_R: 
            ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_1);
            return;

        case AM_R_D8:
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        case AM_D16: {
            u16 lo = bus_read(ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.pc + 1);
            emu_cycles(1);

            ctx.fetched_data = lo | (hi << 8);

            ctx.regs.pc += 2;
            return;
        }

        default:
            printf("Unknown addressing mode: %d, OPCODE: %02X\n", ctx.cur_inst->mode, ctx.cur_opcode);
            exit(-7);
            return;
    }
}

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

        printf("Executing: %02X,   PC: %04X, Data (%02X %02X), Fetched: %04X\n\tA: %02X, F: %02X\n", ctx.cur_opcode, pc, bus_read(pc+1), bus_read(pc+2), 
            ctx.fetched_data, cpu_read_reg(RT_A), cpu_read_reg(RT_F));
        
        if (ctx.cur_inst == NULL) {
            printf("Unknown instruction: %02X\n", ctx.cur_opcode);
            exit(-7);
        }

        execute();
    }

    return true;
}
