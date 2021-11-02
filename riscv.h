//
//  riscv.h
//  RISCV
//
//  Created by Matt Parsons on 13/10/2021.
//

#ifndef riscv_h
#define riscv_h

#include <stdio.h>

enum EXECUTION_MODE{
    RV32I,
    RV32C,
    RV64I
};

typedef struct RISCV_t RISCV_t;

typedef struct RISCV_t{
    
    uint8_t  (*read8)(uint32_t);  // memory handler read(memory array, address)
    uint16_t (*read16)(uint32_t);
    uint32_t (*read32)(uint32_t);
    
    void (*write8)(uint32_t,uint8_t);  // memory handler write(address, value)
    void (*write16)(uint32_t,uint16_t);
    void (*write32)(uint32_t,uint32_t);
    
    void (*interruptRequest)(void);
    
    void (*opCode[128])(RISCV_t*);
    uint32_t xReg[32];
    uint32_t pc;
    
    enum EXECUTION_MODE mode;
    uint32_t* vbr;
    
    //CPU State
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    
    uint32_t func3;
    uint32_t func4;
    uint32_t func7;
    
    uint32_t imm;
    
    uint32_t currentInstruction;
    uint64_t cycle;
} RISCV_t;

RISCV_t* initRISCV(void);

void RISCVExecute(RISCV_t* cpu);
    
#endif /* riscv_h */
