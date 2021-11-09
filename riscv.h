//
//  riscv.h
//  RISCV
//
//  Created by Matt Parsons on 13/10/2021.
//

#ifndef riscv_h
#define riscv_h

#include <stdio.h>

#define R64

//#define print(...) printf(__VA_ARGS__)
#define print(...) if(cpu->dissassem == 1){printf(__VA_ARGS__);}

//#define print

    

enum REG_NAMES{
    zero = 0, ra,sp,gp, tp,t0,t1,t2,s0,s1,a0,
    a1,a2,a3,a4,a5,a6,a7,s2,s3,s4,s5,
    s6,s7,s8,s9,s10,s11,t3,t4,t5,t6
};

enum PRIVILAGE{
    user = 0, supervisor, reserved, machine
};

#ifdef R64
typedef struct{
    uint64_t zero; uint64_t ra; uint64_t sp; uint64_t gp; uint64_t tp; uint64_t t0; uint64_t t1; uint64_t t2; uint64_t s0; uint64_t s1; uint64_t a0;
    uint64_t a1; uint64_t a2; uint64_t a3; uint64_t a4; uint64_t a5; uint64_t a6; uint64_t a7; uint64_t s2; uint64_t s3; uint64_t s4; uint64_t s5;
    uint64_t s6; uint64_t s7; uint64_t s8; uint64_t s9; uint64_t s10; uint64_t s11; uint64_t t3; uint64_t t4; uint64_t t5; uint64_t t6;
} regs_t;
#else
typedef struct{
    uint32_t zero; uint32_t ra; uint32_t sp; uint32_t gp; uint32_t tp; uint32_t t0; uint32_t t1; uint32_t t2; uint32_t s0; uint32_t s1; uint32_t a0;
    uint32_t a1; uint32_t a2; uint32_t a3; uint32_t a4; uint32_t a5; uint32_t a6; uint32_t a7; uint32_t s2; uint32_t s3; uint32_t s4; uint32_t s5;
    uint32_t s6; uint32_t s7; uint32_t s8; uint32_t s9; uint32_t s10; uint32_t s11; uint32_t t3; uint32_t t4; uint32_t t5; uint32_t t6;
} regs_t;
#endif


typedef struct RISCV_t RISCV_t;

typedef struct RISCV_t{
    uint32_t dissassem;
    uint8_t  (*read8)(uint32_t);  // memory handler read(address) => value
    uint16_t (*read16)(uint32_t);
    uint32_t (*read32)(uint32_t);
    uint64_t (*read64)(uint32_t);
    
    void (*write8)(uint32_t,uint8_t);  // memory handler write(address, value)
    void (*write16)(uint32_t,uint16_t);
    void (*write32)(uint32_t,uint32_t);
    void (*write64)(uint32_t,uint64_t);
    
    void (*opCode[128])(RISCV_t*);
    
    
#ifdef R64
    uint64_t xReg[32];
    uint64_t csr[4096];
#else
    uint32_t xReg[32];
    uint32_t csr[4096];
#endif

    
    enum PRIVILAGE privilage;
    uint32_t irqLevelRequest;
    uint32_t halt;                  // wait for an interrupt
    uint32_t iSize;
    uint32_t currentInstruction;
    uint64_t* cycle;
    uint32_t* debug;
    
    uint32_t func3;
    uint32_t func4;
    uint32_t func7;
    
    //CPU State
    enum REG_NAMES rd;
    enum REG_NAMES rs1;
    enum REG_NAMES rs2;
    
    
    uint32_t imm;
    
#ifdef R64
    uint64_t pc;
#else
    uint32_t pc;
#endif
    regs_t* resisters;
    
} RISCV_t;

void RISCVinit(RISCV_t* cpu);

void RISCVSetPC(RISCV_t* cpu, uint32_t pc);

void RISCVSetSP(RISCV_t* cpu, uint32_t sp);

void RISCVExecute(RISCV_t* cpu);

void RISCVInterruptRequest(RISCV_t*cpu, uint32_t level);
    
#endif /* riscv_h */
