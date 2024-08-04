#include <cpu.h>
#include <instructions.h>

instruction instructions[0x100] = {
    // 0x0X
    [0x00] = {INST_NOP},
    [0x01] = {INST_LD, AM_R_D16, RT_BC},
    [0x02] = {INST_LD, AM_MR_R, RT_BC, RT_A},

    [0x05] = {INST_DEC, AM_R, RT_B},
    [0x06] = {INST_LD, AM_R_D8, RT_B},

    [0x08] = {INST_LD, AM_A16_R, RT_NONE, RT_SP},

    [0x0A] = {INST_LD, AM_R_MR, RT_A, RT_BC},

    [0x0E] = {INST_LD, AM_R_D8, RT_C},
    
    // 0x1X

    [0x11] = {INST_LD, AM_R_D16, RT_DE},
    [0x12] = {INST_LD, AM_MR_R, RT_DE, RT_A},

    [0x16] = {INST_LD, AM_R_D8, RT_D},

    [0x1A] = {INST_LD, AM_R_MR, RT_A, RT_DE},

    [0x1E] = {INST_LD, AM_R_D8, RT_E},

    // 0x2X

    [0x21] = {INST_LD, AM_R_D16, RT_HL},
    [0x22] = {INST_LD, AM_HLI_R, RT_HL, RT_A},

    [0x26] = {INST_LD, AM_R_D8, RT_H},

    [0x2A] = {INST_LD, AM_R_HLI, RT_A, RT_HL},

    [0x2E] = {INST_LD, AM_R_D8, RT_L},

    // 0x3X

    [0x31] = {INST_LD, AM_R_D16, RT_SP},
    [0x32] = {INST_LD, AM_HLD_R, RT_HL, RT_A},

    [0x36] = {INST_LD, AM_MR_D8, RT_HL},

    [0x3A] = {INST_LD, AM_R_HLD, RT_A, RT_HL},

    [0x3E] = {INST_LD, AM_R_D8, RT_A},

    // 0x4X
    [0x40] = {INST_LD, AM_R_R, RT_B, RT_B},


    [0x87] = {INST_ADD, AM_R_R, RT_A, RT_A},

    [0xAF] = {INST_XOR, AM_R, RT_A},

    [0xC3] = {INST_JP, AM_D16},

    [0xF3] = {INST_DI}
};

instruction *instruction_by_opcode(u8 opcode) {
    return &instructions[opcode];
}
