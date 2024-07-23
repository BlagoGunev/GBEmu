#include <cpu.h>
#include <emu.h>

static bool check_cond(cpu_context *ctx) {
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch (ctx->cur_inst->cond) {
        CT_NONE: return true;
        CT_C: return c;
        CT_NC: return !c;
        CT_Z: return z;
        CT_NZ: return !z;

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
    // TODO
}

static void proc_jp(cpu_context *ctx) {
    if (check_cond(ctx)) {
        ctx->regs.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}

static INST_PROC processors[] = {
    [INST_NONE] = proc_none,
    [INST_NOP] = proc_nop,
    [INST_LD] = proc_ld,
    [INST_JP] = proc_jp

};

INST_PROC inst_get_processor(inst_type type) {
    return processors[type];
}