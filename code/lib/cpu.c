#include <cpu.h>
#include <bus.h>

cpu_context ctx = {0};

void cpu_init() {

}

static void fetch_instruction() {
    ctx.cur_opcode = bus_read(ctx.regs.pc);
    ctx.regs.pc++;
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);

    if (ctx.cur_inst->type == NULL) {
        printf("Unknown instruction: %02X", ctx.cur_opcode);
        exit(-7);
    }
}

static void fetch_data() {
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    switch(ctx.cur_inst->mode) {
        case AM_IMP: return;

        case AM_R: 
            ctx.fetched_data = cpu_red_reg(ctx.cur_inst->reg_1);
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
        }

        default:
            printf("Unknown addressing mode: %d\n", ctx.cur_inst->mode);
            exit(-7);
            return;
    }
}

static void execute() {
    printf("Not executing yet...\n");
}

bool cpu_step() {
    
    if (!ctx.halted) {
        fetch_instruction();
        fetch_data();
        execute();
    }

    return false;
}
