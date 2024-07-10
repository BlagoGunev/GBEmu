#include <cpu.h>
#include <instructions.h>

instruction instructions[0x100] = {
    [0x00] = {INST_NOP, AM_IMP},

    [0x05] = {INST_DEC, AM_R, RT_B},

    [0x0E] = {INST_LD, AM_R_D8, RT_C},

    [0x87] = {INST_ADD, AM_R_R, RT_A, RT_A},

    [0xAF] = {INST_XOR, AM_R, RT_A},

    [0xC3] = {INST_JP, AM_D16}
};