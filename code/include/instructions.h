#pragma once

#include <common.h>

typedef enum {
    AM_IMP, // implied
    AM_R_D16, // DEST <- SRC
    AM_R_R,
    AM_MR_R,
    AM_R,
    AM_R_D8,
    AM_R_MR,
    AM_R_HLI,
    AM_R_HLD,
    AM_HLI_R,
    AM_HLD_R,
    AM_R_A8,
    AM_A8_R,
    AM_HL_SPR,
    AM_D16,
    AM_D8,
    AM_D16_R,
    AM_MR_D8,
    AM_MR,
    AM_A16_R,
    AM_R_A16
} addr_mode;

typedef enum {
    INST_NONE,
    // x0
    INST_NOP,
    INST_STOP,
    INST_JR,
    INST_LD,
    INST_ADD,
    INST_SUB,
    INST_AND,
    INST_OR,
    INST_RET,
    INST_LDH,
    // x1
    INST_POP,
    // x2
    INST_JP,
    // x3
    INST_INC,
    INST_DI,
    // x4
    INST_CALL,
    // x5
    INST_PUSH,
    // x6
    INST_HALT,
    // x7
    INST_RLCA,
    INST_RLA,
    INST_DAA,
    INST_SCF,
    INST_RST,
    // x8
    INST_ADC,
    INST_SBC,
    INST_XOR,
    INST_CP,
    // x9
    INST_RETI,
    // xB
    INST_PREFIX,
    INST_EI,
    // xD
    INST_DEC,
    // xF
    INST_RRCA,
    INST_RRA,
    INST_CPL,
    INST_CCF,
    // Prefix
    INST_RLC,
    INST_RL,
    INST_SLA,
    INST_SWAP,
    INST_BIT,
    INST_RES,
    INST_SET,
    INST_RRC,
    INST_RR,
    INST_SRA,
    INST_SRL
} inst_type;

typedef enum {
    CT_NONE, CT_NZ, CT_Z, CT_NC, CT_C
} cond_type;

typedef enum {
    RT_NONE,
    RT_A,
    RT_F,
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_AF,
    RT_BC,
    RT_DE,
    RT_HL,
    RT_SP,
    RT_PC
} reg_type;

typedef struct {
    inst_type type;
    addr_mode mode;
    reg_type reg_1;
    reg_type reg_2;
    cond_type cond;
    u8 param;
} instruction;

instruction *instruction_by_opcode(u8 opcode);