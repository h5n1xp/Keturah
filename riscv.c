//
//  riscv.c
//  RISCV
//
//  Created by Matt Parsons on 13/10/2021.
//

#include "riscv.h"

char* abi[] = {
    "zero", " ra ", " sp ", " gp ", " tp ", " t0 ", " t1 ", " t2 ", " s0 ", " s1 ", " a0 ",
    " a1 ", " a2 ", " a3 ", " a4 ", " a5 ", " a6 ", " a7 ", " s2 ", " s3 ", " s4 ", " s5 ",
    " s6 ", " s7 ", " s8 ", " s9 ", " s10", " s11", " t3 ", " t4 ", " t5 ", " t6 "
};




void RMode(RISCV_t* cpu){
    cpu->func7 = (cpu->currentInstruction & 0xFE000000) >>25;
    cpu->func3 = (cpu->currentInstruction & 0x7000)    >> 12;
    cpu->rd    = (cpu->currentInstruction & 0xF80)     >> 7;
    cpu->rs1   = (cpu->currentInstruction & 0xF8000)   >> 15;
    cpu->rs2   = (cpu->currentInstruction & 0x1F00000) >> 20;
}

void IMode(RISCV_t* cpu){
    cpu->func3 = (cpu->currentInstruction & 0x7000)    >> 12;
    cpu->rd    = (cpu->currentInstruction & 0xF80)     >> 7;
    cpu->rs1   = (cpu->currentInstruction & 0xF8000)   >> 15;
    cpu->imm   = (int32_t)(cpu->currentInstruction & 0xFFF00000)>> 20;

}


void SMode(RISCV_t* cpu){
    cpu->func3 = (cpu->currentInstruction & 0x7000)    >> 12;
    cpu->rs1   = (cpu->currentInstruction & 0xF8000)   >> 15;
    cpu->rs2   = (cpu->currentInstruction & 0x1F00000) >> 20;
    cpu->imm   = ((int32_t)(cpu->currentInstruction & 0xFE000000) >> 20) | ((cpu->currentInstruction & 0xF80) >> 7) ;
    
}

void BMode(RISCV_t* cpu){
    cpu->func3 = (cpu->currentInstruction & 0x7000)    >> 12;
    cpu->rs1   = (cpu->currentInstruction & 0xF8000)   >> 15;
    cpu->rs2   = (cpu->currentInstruction & 0x1F00000) >> 20;
    
    cpu->imm = ((((((int32_t)cpu->currentInstruction>> 20) & 0xFFFFFFE0) | ((cpu->currentInstruction >> 7) & 0x0000001F)) & 0xFFFFF7FE) | ((   ((((int32_t)cpu->currentInstruction >> 20) & 0xFFFFFFE0) | ((cpu->currentInstruction >> 7) & 0x0000001F)) & 0x00000001) << 11));
}

void UMode(RISCV_t* cpu){
    cpu->rd    = (cpu->currentInstruction & 0xF80) >> 7;
    cpu->imm   = (cpu->currentInstruction & 0xFFFFF000);
    printf("");
}

void JMode(RISCV_t* cpu){
    cpu->rd    = (cpu->currentInstruction & 0xF80) >> 7;
    //cpu->imm   = ((cpu->currentInstruction & 0xF00) >> 7) | ((cpu->currentInstruction & 0x7E000000) >> 20) | ((cpu->currentInstruction & 0x80) << 4) | ((cpu->currentInstruction & 0x80000000) >> 19);
    
    cpu->imm = ((cpu->currentInstruction & 0x7FE00000) >> 20) | ((cpu->currentInstruction & 0x100000) >> 9) | (cpu->currentInstruction & 0xFF000) | ((int32_t)(cpu->currentInstruction & 0x80000000) >> 11);
}


void CompressedMode(RISCV_t* cpu){
    cpu->func4 = (cpu->currentInstruction & 0xF000) >> 12;
    cpu->func3 = cpu->func4 >> 1;
    cpu->rs1 = (cpu->currentInstruction & 0xF80)  >> 7;
    
    cpu->iSize = 2;
}

void CRMode(RISCV_t* cpu){
    cpu->rs2 = (cpu->currentInstruction & 0x7c)   >> 2;
    cpu->rd  = cpu->rs1;
}

void CIMode(RISCV_t* cpu){
    cpu->imm = ((cpu->currentInstruction & 0x7C) >> 2) | ((int32_t)(cpu->currentInstruction & 0x1000) >> 7);
    cpu->rd = cpu->rs1;
}

void CSMode(RISCV_t* cpu){
    cpu->rd    = (cpu->rd & 7) + 8;
    cpu->rs1   = cpu->rd;
    cpu->rs2   = ((cpu->currentInstruction & 0x1c)   >> 2) + 8;
    cpu->imm = (cpu->currentInstruction & 0x1C00) >> 8 | (cpu->currentInstruction & 0x60) >> 5;
}


void CLMode(RISCV_t* cpu){
    
}

void CIWMode(RISCV_t* cpu){
    
}

void CBMode(RISCV_t* cpu){
    cpu->rs1 =  ((cpu->currentInstruction & 0x380)   >> 7) + 8;
    cpu->rs2 = 0;
    cpu->imm = ((cpu->currentInstruction & 0x7C) >> 2) | ((int32_t)(cpu->currentInstruction & 0x1C00) >> 5);
}


void CBRMode(RISCV_t* cpu){
    cpu->rs1 =  ((cpu->currentInstruction & 0x380)   >> 7) + 8;
    cpu->rs2 = 0;
    
   // cpu->imm 8|4:3|7:6|2:1|5
    
    cpu->imm = (cpu->currentInstruction & 0x1000) << 19; // 8
    cpu->imm = ((int32_t)(cpu->imm & 0x80000000) >> 23); // 8
    
    cpu->imm = cpu->imm | (cpu->currentInstruction & 0x60) << 1; // 7:6
    
    cpu->imm =  cpu->imm | (cpu->currentInstruction & 0x4) << 3; // 5
    
    cpu->imm = cpu->imm | (cpu->currentInstruction & 0xC00) >> 7; // 4:3
    
    cpu->imm = cpu->imm | (cpu->currentInstruction & 0xC18) >> 2; // 2:1
    
    //cpu->imm = ((cpu->currentInstruction & 0x7C) >> 2) | ((int32_t)(cpu->currentInstruction & 0x1C00) >> 5);
}


void TRAP(RISCV_t* cpu){
    
    print("instruction 0x");
    
    if(cpu->iSize == 2){
        print("%0x",cpu->currentInstruction & 65535);
    }else{
        print("%0x",cpu->currentInstruction);
    }
    
    print(" Not implmented\n");
}



// R type Instructions
void RVadd(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] + cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVsub(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] - cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVxor(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] ^ cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVor(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] | cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVand(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] & cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVsll(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] << cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVsrl(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] >> cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVsra(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] >> cpu->xReg[cpu->rs2];
    
    cpu->cycle += 1;
    
}

void RVslt(RISCV_t* cpu){
    
#ifdef R64
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = (int64_t)(cpu->xReg[cpu->rs1] < (int64_t)cpu->xReg[cpu->rs2])?1:0;
    cpu->cycle += 1;
#else
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = (int32_t)(cpu->xReg[cpu->rs1] < (int32_t)cpu->xReg[cpu->rs2])?1:0;
    cpu->cycle += 1;
#endif
}

void RVsltu(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    //I have no fucking clue what this instruction is for.
    cpu->xReg[cpu->rd] = (cpu->xReg[cpu->rs1] < cpu->xReg[cpu->rs2])?1:0;
    cpu->cycle += 1;
    
}








void RVjalr(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    
    cpu->xReg[cpu->rd] = cpu->pc + 4;
    cpu->pc = cpu->xReg[cpu->rs1] + (int32_t)(cpu->imm);
    
    cpu->cycle += 1;
    
}

void OPCode_1100111(RISCV_t* cpu){
    cpu->iSize = 4;
    IMode(cpu);
    print("jalr ");
    RVjalr(cpu);
}


void RVjal(RISCV_t* cpu){
    print("%s, %0x",abi[cpu->rd],cpu->imm + cpu->pc);
    
    cpu->xReg[cpu->rd] = cpu->pc + cpu->iSize;
    cpu->pc += (int32_t)(cpu->imm);
    
    cpu->cycle += 1;
}

void OPCode_1101111(RISCV_t* cpu){
    cpu->iSize = 4;
    JMode(cpu);
    print("jal ");
    RVjal(cpu);
}


void RVlb(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;

    cpu->xReg[cpu->rd] = (uint32_t)cpu->read8(index);
    
    cpu->cycle += 1;
}

void RVlh(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;

    cpu->xReg[cpu->rd] = (uint32_t)cpu->read16(index);
    
    cpu->cycle += 1;
}

void RVlw(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;

    cpu->xReg[cpu->rd] = (int32_t)cpu->read32(index);   //sign extend...
    
    cpu->cycle += 1;
}


void RVlwu(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;

    cpu->xReg[cpu->rd] = (uint32_t)cpu->read32(index);   // don't sign extend...
    
    cpu->cycle += 1;
}

void RVld(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    
    cpu->xReg[cpu->rd] = (uint64_t)cpu->read64(index);
    
    cpu->cycle += 1;
}


void OPCode_0000011(RISCV_t* cpu){
    cpu->iSize = 4;
    IMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            print("lb ");
            RVlb(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 1:
            print("lh ");
            RVlh(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 2:
            print("lw ");
            RVlw(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 3:
#ifdef R64
            print("ld ");
            RVld(cpu);
            cpu->pc += cpu->iSize;
#else
            TRAP(cpu);
#endif
            break;
            
        case 4:
            print("lbu ");
            TRAP(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 5:
            print("lhu ");
            TRAP(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 6:
            print("lwu");
            RVlwu(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        default :
            TRAP(cpu);
            break;
    }
    
}


void RVauipc(RISCV_t* cpu){
    print("%s,%d",abi[cpu->rd],cpu->imm >> 12);
    
    cpu->xReg[cpu->rd] = (cpu->pc) + (cpu->imm);
}

void OPCode_0010111(RISCV_t* cpu){
    cpu->iSize = 4;
    UMode(cpu);
    print("auipc ");
    RVauipc(cpu);
}


void RVaddi(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    
    

    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    cpu->cycle += 1;

}

void RVslli(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    
#ifdef R64
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] << (cpu->imm & 0x3F);
    cpu->cycle += 1;
#else
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] << (cpu->imm & 0x1F);
    cpu->cycle += 1;
#endif
    
}

void RVslti(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
#ifdef R64
    cpu->xReg[cpu->rd] = ((int64_t)cpu->xReg[cpu->rs1] < (int64_t)cpu->imm) ? 1:0;
    cpu->cycle += 1;
#else
    cpu->xReg[cpu->rd] = ((int32_t)cpu->xReg[cpu->rs1] < (int32_t)cpu->imm) ? 1:0;
    cpu->cycle += 1;
#endif
}

void RVsltiu(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = (cpu->xReg[cpu->rs1] < cpu->imm) ? 1:0;
    cpu->cycle += 1;
}

void RVxori(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] ^ cpu->imm;
    cpu->cycle += 1;
}


void RVsrai(RISCV_t* cpu){
    
#ifdef R64
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = (int64_t)cpu->xReg[cpu->rs1] >> (cpu->imm & 0x3F);
    cpu->cycle += 1;
#else
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] >> (cpu->imm & 0x1F);
    cpu->cycle += 1;
#endif
}

void RVsrli(RISCV_t* cpu){
    
#ifdef R64
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] >> (cpu->imm & 0x3F);
    cpu->cycle += 1;
#else
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] >> (cpu->imm & 0x1F);
    cpu->cycle += 1;
#endif
}

void RVori(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] | cpu->imm;
    cpu->cycle += 1;
}

void RVandi(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] & cpu->imm;
    cpu->cycle += 1;
}

void OPCode_0010011(RISCV_t* cpu){
    cpu->iSize = 4;
    IMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            print("addi ");
            RVaddi(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 1:
            print("slli ");
            RVslli(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 2:
            print("slti ");
            RVslti(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 3:
            print("sltiu ");
            RVsltiu(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 4:
            print("xori ");
            RVxori(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 5:
            if( (cpu->imm & 0x400) == 0x400){
                print("srai ");
                RVsrai(cpu);
            }else{
                print("srli ");
                RVsrli(cpu);
            }
            cpu->pc += cpu->iSize;
            break;
        case 6:
            print("ori ");
            RVori(cpu);
            cpu->pc += cpu->iSize;
            break;
        case 7:
            print("andi ");
            RVandi(cpu);
            cpu->pc += cpu->iSize;
            break;
            
    }
    
}


void RVaddiw(RISCV_t* cpu){
    

    
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    cpu->cycle += 1;
}

void RVslliw(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->imm],abi[cpu->rs1],cpu->rs2);
    
    //nasty ISA hack... the rs2 value used as a the shift amount, and imm used as rd?
    cpu->rd = cpu->imm;
    cpu->imm = cpu->rs2;
    
    cpu->xReg[cpu->rd] = (uint32_t)cpu->xReg[cpu->rs1] << cpu->imm;
    cpu->cycle += 1;
}

void RVsrliw(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->imm],abi[cpu->rs1],cpu->rs2);
    
    //nasty ISA hack... the rs2 value used as a the shift amount, and imm used as rd?
    cpu->rd = cpu->imm;
    cpu->imm = cpu->rs2;
    
    cpu->xReg[cpu->rd] = (uint32_t)cpu->xReg[cpu->rs1] >> cpu->imm;
    cpu->cycle += 1;
}

void RVsraiw(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->imm],abi[cpu->rs1],cpu->rs2);
    
    //nasty ISA hack... the rs2 value used as a the shift amount, and imm used as rd?
    cpu->rd = cpu->imm;
    cpu->imm = cpu->rs2;
    
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] >> cpu->imm;
    cpu->cycle += 1;
}

void OPCode_0011011(RISCV_t* cpu){
    cpu->iSize = 4;
    
    SMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            IMode(cpu);
        
            print("addiw ");
            RVaddiw(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 1:
            print("slliw ");
            RVslliw(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 5:
            
            if(cpu->func7 == 32){
                print("sraiw ");
                RVsraiw(cpu);
            }else{
                print("srliw ");
                RVsrliw(cpu);
            }
            
            cpu->pc += cpu->iSize;
            break;
            
        default:
            TRAP(cpu);
            break;
            
    }
    
    

}

void RVsubw(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] - (int32_t)cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void RVaddw(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}


void OPCode_0111011(RISCV_t* cpu){
    cpu->iSize = 4;
    RMode(cpu);
    
    switch(cpu->func3){
        case 0:
            if( (cpu->func7 & 127) == 32){
                print("subw ");
                RVsubw(cpu);
            }else{
                print("addw ");
                RVaddw(cpu);
            }
            cpu->pc += cpu->iSize;
            break;
        default:
            TRAP(cpu);
            break;
            
    }
    
 
}

void RVsb(RISCV_t* cpu){
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    
    cpu->write8(index,(uint8_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
    
}

void RVsh(RISCV_t* cpu){
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    
    cpu->write16(index,(uint16_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
    
}

void RVsw(RISCV_t* cpu){
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    
    cpu->write32(index,(uint32_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
    
}

void RVsd(RISCV_t* cpu){
  
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = (int32_t)cpu->xReg[cpu->rs1] + (int32_t)cpu->imm;
    
    cpu->write64(index,(uint64_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
}

void OPCode_0100011(RISCV_t* cpu){
    cpu->iSize = 4;
    SMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            print("sb ");
            RVsb(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 1:
            print("sh ");
            RVsh(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 2:
            
            print("sw ");
            RVsw(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 3:
#ifdef R64
            print("sd");
            RVsd(cpu);
            cpu->pc += cpu->iSize;
#else
            TRAP(cpu)
#endif
            break;
            
        default:
            TRAP(cpu);
            break;
            
    }
    
    
}


void RVlui(RISCV_t* cpu){
    print("%s,%d",abi[cpu->rd],cpu->imm >> 12);
    
    cpu->xReg[cpu->rd] = (int32_t)cpu->imm;
    cpu->cycle += 1;
}

void OPCode_0110111(RISCV_t* cpu){
    cpu->iSize = 4;
    UMode(cpu);
    print("lui ");
    RVlui(cpu);
    cpu->pc += cpu->iSize;
}

void OPCode_0110011(RISCV_t* cpu){
    cpu->iSize = 4;
    RMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            if( cpu->func7 == 0x20){
                print("sub ");
                RVsub(cpu);
            }else{
                print("add ");
                RVadd(cpu);
            }
            cpu->pc += cpu->iSize;
            break;
            
        case 1:
            print("sll ");
            RVsll(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 2:
            print("slt ");
            RVslt(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 3:
            print("sltu ");
            RVsltu(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 4:
            print("xor ");
            RVxor(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 5:
            if( cpu->func7 == 0x20){
                print("sra ");
                RVsra(cpu);
            }else{
                print("srl ");
                RVsrl(cpu);
            }
            cpu->pc += cpu->iSize;
            break;
            
        case 6:
            print("or ");
            RVor(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 7:
            print("and ");
            RVand(cpu);
            cpu->pc += cpu->iSize;
            break;
    }
    
    
}







void RVbeq(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] == cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
    
    cpu->cycle += 1;
    
}

void RVbne(RISCV_t* cpu){
    print("%s, %s, %x",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] != cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
    
    cpu->cycle += 1;
}

void RVblt(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
#ifdef R64
    if((int64_t)cpu->xReg[cpu->rs1] < (int64_t)cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
#else
    if((int32_t)cpu->xReg[cpu->rs1] < (int32_t)cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
#endif
    cpu->cycle += 1;
}


void RVbge(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
#ifdef R64
    if((int64_t)cpu->xReg[cpu->rs1] >= (int64_t)cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
#else
    if((int32_t)cpu->xReg[cpu->rs1] >= (int32_t)cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
#endif
    cpu->cycle += 1;
}

void RVbltu(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] < cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
    
    cpu->cycle += 1;
    
}

void RVbgeu(RISCV_t* cpu){
    print("%s, %s, %x",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] >= cpu->xReg[cpu->rs2]){
        
        cpu->pc += (int32_t)cpu->imm;
        
    }else{
        cpu->pc += cpu->iSize;
    }
    
    cpu->cycle += 1;
}

void OPCode_1100011(RISCV_t* cpu){
    cpu->iSize = 4;
    BMode(cpu);
    
    switch(cpu->func3){
        case 0:
            print("beq ");
            RVbeq(cpu);
            break;
        case 1:
            print("bne ");
            RVbne(cpu);
            break;
        case 2:
            TRAP(cpu);
            break;
        case 3:
            print("illegal instruction ");
            TRAP(cpu);
            break;
        case 4:
            print("blt ");
            RVblt(cpu);
            break;
        case 5:
            print("bge ");
            RVbge(cpu);
            break;
        case 6:
            print("bltu ");
            RVbltu(cpu);
            break;
        case 7:
            print("bgeu ");
            RVbgeu(cpu);
            break;
    }
    
}



void OPCode_1110011(RISCV_t* cpu){
    cpu->iSize = 4;
    RMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            
            switch(cpu->rs2){
                    
                case 0:
                    print("ecall ");
                    TRAP(cpu);
                    break;
                    
                case 1:
                    print("ebreak ");
                    TRAP(cpu);
                    cpu->pc += 4;
                    break;
                    
                case 2:
            
                    switch(cpu->func7){
            
                        case 0:
                            print("uret ");
                            TRAP(cpu);
                            break;
                    
                        case 8:
                            print("sret ");
                            TRAP(cpu);
                            break;
                    
                        case 16:
                            print("hret ");
                            TRAP(cpu);
                            break;
                    
                        case 24:
                            print("mret ");
                            TRAP(cpu);
                            break;
                            
                        default:
                            TRAP(cpu);
                            break;
                    }
                    break;
                    
                case 4:
                    printf("sfence ");
                    TRAP(cpu);
                    break;
                    
                case 5:
                    printf("wfi ");
                    //Wait For Interrupt... same as hlt on x86 I guess...
                    TRAP(cpu);
                    break;
                    
                case 18:
                    print("dret ");
                    TRAP(cpu);
                    break;
                    
                default:
                    TRAP(cpu);
                    break;
            }
            break;
            
            
            
            
        default:
            TRAP(cpu);
            break;
    }
    
}


//************************************** Compressed RV32C
//OPCodes

void OPCode_00(RISCV_t* cpu){
    CompressedMode(cpu);

    
    switch(cpu->func3){
        case 0:
            CIWMode(cpu);
            print("*c.addi4spn ");
            //print("%s, %d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rd]);
            break;
            
        case 1:
            TRAP(cpu);
            break;
            
        case 2:
            CLMode(cpu);
            print("*c.lw ");
            //print("%s, %d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rd]);
            break;
            
        case 3:
            TRAP(cpu);
            break;
            
        case 4:
            TRAP(cpu);
            break;
            
        case 5:
            TRAP(cpu);
            break;
            
        case 6:
            CSMode(cpu);
            print("*c.sw ");
            print("%s, %d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rd]);
            break;
            
    }
    

    
}

void OPCode_01(RISCV_t* cpu){
    CompressedMode(cpu);


    
    switch(cpu->func3){
            
        case 0:
            TRAP(cpu);
            break;
            
        case 1:
            TRAP(cpu);
            break;
            
        case 2:
            CIMode(cpu);
            print("c.li ");
            RVaddi(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 3:
            TRAP(cpu);
            break;
            
        case 4:
            
            CBMode(cpu);
            
            if( (cpu->imm & 0xE0) == 0){
                cpu->rd = cpu->rs1;
                print("c.srli");
                RVsrli(cpu);
                cpu->pc += cpu->iSize;
                break;
            }
            
            if( (cpu->imm & 0xE0) == 0x20){
                cpu->rd = cpu->rs1;
                print("c.srai");
                RVsrai(cpu);
                cpu->pc += cpu->iSize;
                break;
            }
            
            if( (cpu->imm & 0x60) == 0x40){
                cpu->rd = cpu->rs1;
                cpu->imm = cpu->imm | (cpu->currentInstruction & 0x1000) >> 7;
                print("c.andi");
                RVandi(cpu);
                cpu->pc += cpu->iSize;
                break;
            }
            
            CSMode(cpu);
            
            switch(cpu->imm){
                case 12:
                    print("c.sub ");
                    RVsub(cpu);
                    break;
                    
                case 13:
                    print("c.xor ");
                    RVxor(cpu);
                    break;
                    
                case 14:
                    print("c.or ");
                    RVor(cpu);
                    break;
                    
                case 15:
                    print("c.and ");
                    RVand(cpu);
                    break;
                default:
                    TRAP(cpu);
                    break;
                    
            }
            break;
            
        case 5:
            TRAP(cpu);
            break;
            
        case 6:
            CBRMode(cpu);
            
           // cpu->imm 8|4:3|7:6|2:1|5
            
            print("c.beqz ");
            RVbeq(cpu);

            break;
            
        case 7:
            CBRMode(cpu);
            print("c.bnez ");
            RVbne(cpu);
            break;
    }
    
}

void OPCode_10(RISCV_t* cpu){
    CompressedMode(cpu);
    
    switch(cpu->func3){
        case 0:
            CIMode(cpu);
            print("c.slli ");
            cpu->imm = cpu->imm & 0x1F;
            RVslli(cpu);
            cpu->pc += cpu->iSize;
            return;
            
        default:
           // TRAP(cpu);
            break;
    }
    
    CRMode(cpu);
    switch(cpu->func4){
            

        case 8:
            
            if( (cpu->rs1 != 0) && (cpu->rs2 == 0) ){
                cpu->rd = cpu->currentInstruction & 0x1000 >> 12;
                cpu->imm = 0;
                print("c.jalr ");
                RVjalr(cpu);
                break;
            }
            
            print("c.mv");
            cpu->rs1 = 0;
            RVadd(cpu);
            cpu->pc += cpu->iSize;
            break;
            
        case 9:
            print("c.add ");
            RVadd(cpu);
            cpu->pc += cpu->iSize;
            break;
        default:
            TRAP(cpu);
            break;
            
    }
    
}







void interruptRequest(void){
    
}


void RISCVSetPC(RISCV_t* cpu, uint32_t pc){
    cpu->pc = pc;
}

void RISCVSetSP(RISCV_t* cpu, uint32_t sp){
    cpu->xReg[2] = sp;
}

void RISCVinit(RISCV_t* cpu){
    

    cpu->cycle = 0;
    cpu->pc = 0;

    cpu->interruptRequest = interruptRequest;
    
    

    for(int i=0; i<128;++i){
        cpu->opCode[i] = TRAP;
    }
    
    
    
    //Build CPU OP Code table
    
    //RV32C set
    
    for(int i=0; i < 128; ++i){
        
        if( (i & 3) == 0){
            cpu->opCode[i] = OPCode_00;
        }
        
        if( (i & 3) == 1){
            cpu->opCode[i] = OPCode_01;
        }
        
        if( (i & 3) == 2){
            cpu->opCode[i] = OPCode_10;
        }
        
        //Trap any unimplemented OPCodes
        if( (i & 3) == 3){
            cpu->opCode[i] = TRAP;
        }
        
    }
    
    //RV32I & RV64I set
    cpu->opCode[3]   = OPCode_0000011;
    cpu->opCode[19]  = OPCode_0010011;
    cpu->opCode[23]  = OPCode_0010111;
    cpu->opCode[27]  = OPCode_0011011;
    cpu->opCode[35]  = OPCode_0100011;
    cpu->opCode[51]  = OPCode_0110011;
    cpu->opCode[55]  = OPCode_0110111;
    cpu->opCode[59]  = OPCode_0111011;
    cpu->opCode[99]  = OPCode_1100011;
    cpu->opCode[103] = OPCode_1100111;
    cpu->opCode[111] = OPCode_1101111;
    cpu->opCode[115] = OPCode_1110011;
    
    

    //All registers 0 - 31 default to 0
    for(int i=1;i<32;i++){
        cpu->xReg[i] = 0;
    }

    cpu->resisters = (regs_t*)&cpu->xReg[0]; // human readable form of the registers for debugging
}





void RISCVExecute(RISCV_t* cpu){
    
    
    //Check interrupts
    
    
    //Fetch instruction
    print("%d | %0x: ",cpu->cycle, cpu->pc);
    cpu->currentInstruction = cpu->read32(cpu->pc);
    
    
    //Decode & Execute
    
    if(cpu->pc == 0x100028){
        printf("");
    }
    
    cpu->opCode[cpu->currentInstruction & 127](cpu);
    
    //Set xReg0 to 0 after every instruction, to discard any data stored there.
    cpu->xReg[0] = 0;

    
    print("\n");
}



