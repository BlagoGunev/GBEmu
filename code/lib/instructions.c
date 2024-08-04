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
    [0x41] = {INST_LD, AM_R_R, RT_B, RT_C},
    [0x42] = {INST_LD, AM_R_R, RT_B, RT_D},
    [0x43] = {INST_LD, AM_R_R, RT_B, RT_E},
    [0x44] = {INST_LD, AM_R_R, RT_B, RT_H},
    [0x45] = {INST_LD, AM_R_R, RT_B, RT_L},
    [0x46] = {INST_LD, AM_R_MR, RT_B, RT_HL},
    [0x47] = {INST_LD, AM_R_R, RT_B, RT_A},
    [0x48] = {INST_LD, AM_R_R, RT_C, RT_B},
    [0x49] = {INST_LD, AM_R_R, RT_C, RT_C},
    [0x4A] = {INST_LD, AM_R_R, RT_C, RT_D},
    [0x4B] = {INST_LD, AM_R_R, RT_C, RT_E},
    [0x4C] = {INST_LD, AM_R_R, RT_C, RT_H},
    [0x4D] = {INST_LD, AM_R_R, RT_C, RT_L},
    [0x4E] = {INST_LD, AM_R_MR, RT_C, RT_HL},
    [0x4F] = {INST_LD, AM_R_R, RT_C, RT_A},

    // 0x5X
    [0x50] = {INST_LD, AM_R_R, RT_D, RT_B},
    [0x51] = {INST_LD, AM_R_R, RT_D, RT_C},
    [0x52] = {INST_LD, AM_R_R, RT_D, RT_D},
    [0x53] = {INST_LD, AM_R_R, RT_D, RT_E},
    [0x54] = {INST_LD, AM_R_R, RT_D, RT_H},
    [0x55] = {INST_LD, AM_R_R, RT_D, RT_L},
    [0x56] = {INST_LD, AM_R_MR, RT_D, RT_HL},
    [0x57] = {INST_LD, AM_R_R, RT_D, RT_A},
    [0x58] = {INST_LD, AM_R_R, RT_E, RT_B},
    [0x59] = {INST_LD, AM_R_R, RT_E, RT_C},
    [0x5A] = {INST_LD, AM_R_R, RT_E, RT_D},
    [0x5B] = {INST_LD, AM_R_R, RT_E, RT_E},
    [0x5C] = {INST_LD, AM_R_R, RT_E, RT_H},
    [0x5D] = {INST_LD, AM_R_R, RT_E, RT_L},
    [0x5E] = {INST_LD, AM_R_MR, RT_E, RT_HL},
    [0x5F] = {INST_LD, AM_R_R, RT_E, RT_A},

    // 0x6X
    [0x60] = {INST_LD, AM_R_R, RT_H, RT_B},
    [0x61] = {INST_LD, AM_R_R, RT_H, RT_C},
    [0x62] = {INST_LD, AM_R_R, RT_H, RT_D},
    [0x63] = {INST_LD, AM_R_R, RT_H, RT_E},
    [0x64] = {INST_LD, AM_R_R, RT_H, RT_H},
    [0x65] = {INST_LD, AM_R_R, RT_H, RT_L},
    [0x66] = {INST_LD, AM_R_MR, RT_H, RT_HL},
    [0x67] = {INST_LD, AM_R_R, RT_H, RT_A},
    [0x68] = {INST_LD, AM_R_R, RT_L, RT_B},
    [0x69] = {INST_LD, AM_R_R, RT_L, RT_C},
    [0x6A] = {INST_LD, AM_R_R, RT_L, RT_D},
    [0x6B] = {INST_LD, AM_R_R, RT_L, RT_E},
    [0x6C] = {INST_LD, AM_R_R, RT_L, RT_H},
    [0x6D] = {INST_LD, AM_R_R, RT_L, RT_L},
    [0x6E] = {INST_LD, AM_R_MR, RT_L, RT_HL},
    [0x6F] = {INST_LD, AM_R_R, RT_L, RT_A},

    // 0x7X
    [0x70] = {INST_LD, AM_MR_R, RT_HL, RT_B},
    [0x71] = {INST_LD, AM_MR_R, RT_HL, RT_C},
    [0x72] = {INST_LD, AM_MR_R, RT_HL, RT_D},
    [0x73] = {INST_LD, AM_MR_R, RT_HL, RT_E},
    [0x74] = {INST_LD, AM_MR_R, RT_HL, RT_H},
    [0x75] = {INST_LD, AM_MR_R, RT_HL, RT_L},
    [0x76] = {INST_HALT},
    [0x77] = {INST_LD, AM_MR_R, RT_HL, RT_A},
    [0x78] = {INST_LD, AM_R_R, RT_A, RT_B},
    [0x79] = {INST_LD, AM_R_R, RT_A, RT_C},
    [0x7A] = {INST_LD, AM_R_R, RT_A, RT_D},
    [0x7B] = {INST_LD, AM_R_R, RT_A, RT_E},
    [0x7C] = {INST_LD, AM_R_R, RT_A, RT_H},
    [0x7D] = {INST_LD, AM_R_R, RT_A, RT_L},
    [0x7E] = {INST_LD, AM_R_MR, RT_A, RT_HL},
    [0x7F] = {INST_LD, AM_R_R, RT_A, RT_A},

    [0x87] = {INST_ADD, AM_R_R, RT_A, RT_A},

    [0xAF] = {INST_XOR, AM_R, RT_A},

    [0xC3] = {INST_JP, AM_D16},

    // 0xEX

    [0xE2] = {INST_LD, AM_MR_R, RT_C, RT_A},

    [0xEA] = {INST_LD, AM_A16_R, RT_NONE, RT_A},

    // 0xFX

    [0xF2] = {INST_LD, AM_R_MR, RT_A, RT_C},
    [0xF3] = {INST_DI},

    [0xF8] = {INST_LD, AM_HL_SPR, RT_HL},
    [0xF9] = {INST_LD, AM_R_R, RT_SP, RT_HL},
    [0xFA] = {INST_LD, AM_R_A16, RT_A}

};

instruction *instruction_by_opcode(u8 opcode) {
    return &instructions[opcode];
}
