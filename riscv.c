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


#define print(...) printf(__VA_ARGS__)
//#define print

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
    cpu->imm   = (cpu->currentInstruction & 0xFFF00000)>> 20;
    
    //Sign extend
    if(cpu->imm & 0x800){
        cpu->imm = 0xFFFFF000 | cpu->imm;
    }
}


void SMode(RISCV_t* cpu){
    cpu->func3 = (cpu->currentInstruction & 0x7000)    >> 12;
    cpu->rs1   = (cpu->currentInstruction & 0xF8000)   >> 15;
    cpu->rs2   = (cpu->currentInstruction & 0x1F00000) >> 20;
    cpu->imm   = ((cpu->currentInstruction & 0xFE000000) >> 20) | ((cpu->currentInstruction & 0xF80) >> 7) ;
    
    //Sign extend
    if(cpu->imm & 0x800){
        cpu->imm = 0xFFFFF000 | cpu->imm;
    }

}

void BMode(RISCV_t* cpu){
    cpu->func3 = (cpu->currentInstruction & 0x7000)    >> 12;
    cpu->rs1   = (cpu->currentInstruction & 0xF8000)   >> 15;
    cpu->rs2   = (cpu->currentInstruction & 0x1F00000) >> 20;

    /*
    uint32_t eleven = (cpu->currentInstruction & 0x80) << 4;
    uint32_t lower = (cpu->currentInstruction & 0xF00) >> 7;
    uint32_t upper  = (cpu->currentInstruction & 0x7E000000) >> 20;
    uint32_t twelve = (cpu->currentInstruction & 0x80000000) >> 19;
    
    cpu->imm = twelve|eleven|upper|lower;
    */
    
    cpu->imm = ((((((int32_t)cpu->currentInstruction>> 20) & 0xFFFFFFE0) | ((cpu->currentInstruction >> 7) & 0x0000001F)) & 0xFFFFF7FE) | ((   ((((int32_t)cpu->currentInstruction >> 20) & 0xFFFFFFE0) | ((cpu->currentInstruction >> 7) & 0x0000001F)) & 0x00000001) << 11));
    //print("%d, %d",imm2,imm3);
}

void UMode(RISCV_t* cpu){
    cpu->rd    = (cpu->currentInstruction & 0xF80) >> 7;
    cpu->imm   = ((cpu->currentInstruction & 0xFFFFF000) >> 12);
}

void JMode(RISCV_t* cpu){
    cpu->rd    = (cpu->currentInstruction & 0xF80) >> 7;
    //cpu->imm   = ((cpu->currentInstruction & 0xF00) >> 7) | ((cpu->currentInstruction & 0x7E000000) >> 20) | ((cpu->currentInstruction & 0x80) << 4) | ((cpu->currentInstruction & 0x80000000) >> 19);
    
    cpu->imm = ((cpu->currentInstruction & 0x7FE00000) >> 20) | ((cpu->currentInstruction & 0x100000) >> 9) | (cpu->currentInstruction & 0xFF000) | (((int32_t)cpu->currentInstruction & 0x80000000) >> 11);
    
}


void CompressedMode(RISCV_t* cpu){
    cpu->func4 = (cpu->currentInstruction & 0xF000) >> 12;
    cpu->func3 = cpu->func4 >> 1;
    cpu->rs1 = (cpu->currentInstruction & 0xF80)  >> 7;
    
}

void CRMode(RISCV_t* cpu){
    cpu->rs2 = (cpu->currentInstruction & 0x7c)   >> 2;
    cpu->rd  = cpu->rs1;
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

void doNothing(RISCV_t* cpu){
    print("Do Nothing!");
}

void doSomething(RISCV_t* cpu){
    print("Do Something");
}

void doSomethingElse(RISCV_t* cpu){
    print("Do Something Else");
}

void resetPC(RISCV_t* cpu){
    print("Reset PC");
    cpu->pc = 0;
}


void TRAP(RISCV_t* cpu){
    
    print("instruction 0x");
    
    if(cpu->mode == RV32C){
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
    /*
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] + cpu->xReg[cpu->rs2];
    
    // if negative we need to extended the sign bits
    if( (cpu->xReg[cpu->rs1] & 0x80000000) == 0x80000000){
        
        uint32_t ext = 0xFFFFFFFF << (31 - cpu->xReg[cpu->rs2]);
        cpu->xReg[cpu->rd] |= ext;
    }
    */
    
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] >> cpu->xReg[cpu->rs2];
    
    cpu->cycle += 1;
    
}

void RVslt(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = (cpu->xReg[cpu->rs1] < cpu->xReg[cpu->rs2])?1:0;
    cpu->cycle += 1;
    
}

void RVsltu(RISCV_t* cpu){
    print("%s, %s, %s",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    //I have no fucking clue what this instruction is for.
    cpu->xReg[cpu->rd] = (cpu->xReg[cpu->rs1] < cpu->xReg[cpu->rs2])?1:0;
    cpu->cycle += 1;
    
}





//OPCodes

void OPCode_00(RISCV_t* cpu){

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
 

    
    switch(cpu->func3){
            
        case 0:
            TRAP(cpu);
            break;
            
        case 1:
            TRAP(cpu);
            break;
            
        case 2:
            TRAP(cpu);
            break;
            
        case 3:
            TRAP(cpu);
            break;
            
        case 4:
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
            TRAP(cpu);
            break;
            
        case 7:
            TRAP(cpu);
            break;
    }
    
}

void OPCode_10(RISCV_t* cpu){
    CRMode(cpu);
    
    switch(cpu->func4){
            
        case 8:
            print("c.mv");
            cpu->rs1 = 0;
            RVadd(cpu);
            break;
            
        case 9:
            print("c.add ");
            RVadd(cpu);
            break;
        default:
            TRAP(cpu);
            break;
            
    }
    
}


void RVjalr(RISCV_t* cpu){
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm + cpu->pc,abi[cpu->rs1]);
    
    cpu->xReg[cpu->rd] = cpu->pc; //Don't need to add 4 to the return address as any instruction which loads this value into PC will add in the decoder stage.
    cpu->pc += cpu->xReg[cpu->rs1] + (cpu->imm);
    
    cpu->cycle += 1;
    
}

void OPCode_1100111(RISCV_t* cpu){
    IMode(cpu);
    print("jalr ");
    RVjalr(cpu);
    
}


void RVjal(RISCV_t* cpu){
    print("%s, %d",abi[cpu->rd],cpu->imm + cpu->pc);
    
    cpu->xReg[cpu->rd] = cpu->pc; //Don't need to add 4 to the return address as any instruction which loads this value into PC will add in the decoder stage.
    cpu->pc += (cpu->imm);
    
    cpu->cycle += 1;
}

void OPCode_1101111(RISCV_t* cpu){
    JMode(cpu);
    print("jal ");
    RVjal(cpu);
}


void RVlb(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];

    cpu->xReg[cpu->rd] = (uint32_t)cpu->read8(index);
    
    cpu->cycle += 1;
}

void RVlh(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];

    cpu->xReg[cpu->rd] = (uint32_t)cpu->read16(index);
    
    cpu->cycle += 1;
}

void RVlw(RISCV_t* cpu){
    
    print("%s, %d(%s)",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];

    cpu->xReg[cpu->rd] = (uint32_t)cpu->read32(index);
    
    cpu->cycle += 1;
}

void RVld(RISCV_t* cpu){
    
    print("%s, %d(%s) WARNING: NOT 64bit",abi[cpu->rd],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];

    //cpu->debug = cpu->vbr + (index / 4);
    
    cpu->xReg[cpu->rd] = (uint32_t)cpu->read32(index);
    
    cpu->cycle += 1;
}


void OPCode_0000011(RISCV_t* cpu){
    
    IMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            print("lb ");
            RVlb(cpu);
            break;
        case 1:
            print("lh ");
            RVlh(cpu);
            break;
        case 2:
            print("lw ");
            RVlw(cpu);
            break;
        case 3:
            print("ld ");
            cpu->mode = RV64I;
            RVld(cpu);
            break;
            
        case 4:
            print("lbu ");
            break;
        case 5:
            print("lhu ");
            break;
    }
    
}


void RVauipc(RISCV_t* cpu){
    print("%s,%d",abi[cpu->rd],cpu->imm);
    
    cpu->xReg[cpu->rd] = (cpu->pc - 4) + (cpu->imm << 12); // minus 4 from the pc as the decoder will add 4.
}

void OPCode_0010111(RISCV_t* cpu){
    UMode(cpu);
    print("auipc ");
    RVauipc(cpu);
}


void RVaddi(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    
    //need to be signed
    //int32_t rs1 = (int32_t)cpu->xReg[cpu->rs1];
    //int32_t imm = (int32_t)cpu->imm;
    //int32_t val = rs1 + imm;
    
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] + cpu->imm;
    cpu->cycle += 1;
}

void RVslli(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] << (cpu->imm & 0x1F);
    cpu->cycle += 1;
}

void RVslti(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = (cpu->xReg[cpu->rs1] < cpu->imm) ? 1:0;
    cpu->cycle += 1;
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
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = (int32_t)cpu->xReg[cpu->rs1] >> (cpu->imm & 0x1F);
    cpu->cycle += 1;
}

void RVsrli(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] >> (cpu->imm & 0x1F);
    cpu->cycle += 1;
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
    
    IMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            print("addi ");
            RVaddi(cpu);
            break;
        case 1:
            print("slli ");
            RVslli(cpu);
            break;
        case 2:
            print("slti ");
            RVslti(cpu);
            break;
        case 3:
            print("sltiu ");
            RVsltiu(cpu);
            break;
        case 4:
            print("xori ");
            RVxori(cpu);
            break;
        case 5:
            if( (cpu->imm & 0x200) == 0x200){
                print("srai ");
                RVsrai(cpu);
            }else{
                print("srli ");
                RVsrli(cpu);
            }
            break;
        case 6:
            print("ori ");
            RVori(cpu);
            break;
        case 7:
            print("andi ");
            RVandi(cpu);
            break;
            
    }
    
}


void RVaddiw(RISCV_t* cpu){
    print("%s, %s, %d WARNING: NOT 64bit",abi[cpu->rd],abi[cpu->rs1],cpu->imm);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] + cpu->imm;
    cpu->cycle += 1;
}

void RVslliw(RISCV_t* cpu){
    print("%s, %s, %d WARNING: NOT 64bit",abi[cpu->imm],abi[cpu->rs1],cpu->rs2);
    
    //nasty ISA hack... the rs2 value used as a the shift amount, and imm used as rd?
    cpu->rd = cpu->imm;
    cpu->imm = cpu->rs2;
    
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] << cpu->imm;
    cpu->cycle += 1;
}




void OPCode_0011011(RISCV_t* cpu){
    cpu->mode = RV64I;
    
    SMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            IMode(cpu);
            print("addiw ");
            RVaddiw(cpu);
            break;
            
        case 1:
            print("slliw ");
            RVslliw(cpu);
            break;
            
        default:
            TRAP(cpu);
            break;
            
    }
    
    

}

void RVaddw(RISCV_t* cpu){
    print("%s, %s, %s WARNING: NOT 64bit ",abi[cpu->rd],abi[cpu->rs1],abi[cpu->rs2]);
    cpu->xReg[cpu->rd] = cpu->xReg[cpu->rs1] + cpu->xReg[cpu->rs2];
    cpu->cycle += 1;
}

void OPCode_0111011(RISCV_t* cpu){
    
    RMode(cpu);
    cpu->mode = RV64I;
    
    switch(cpu->func3){
        case 0:
            if( (cpu->func7 & 127) == 8){
                print("subw ");
            }else{
                print("addw ");
                RVaddw(cpu);
            }
            break;
        default:
            TRAP(cpu);
            break;
            
    }
    
 
}

void RVsb(RISCV_t* cpu){
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];
    
    cpu->write8(index,(uint8_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
    
}

void RVsh(RISCV_t* cpu){
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];
    
    cpu->write16(index,(uint16_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
    
}

void RVsw(RISCV_t* cpu){
    print("%s,%d(%s)",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];
    
    cpu->write32(index,(uint32_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
    
}

void RVsd(RISCV_t* cpu){
  
    print("%s,%d(%s) WARNING: NOT 64 BIT",abi[cpu->rs2],cpu->imm,abi[cpu->rs1]);
    
    uint32_t index = cpu->imm + cpu->xReg[cpu->rs1];
    
    cpu->write32(index,(uint32_t)cpu->xReg[cpu->rs2]);
    
    cpu->cycle += 1;
}

void OPCode_0100011(RISCV_t* cpu){
    
    SMode(cpu);
    
    switch(cpu->func3){
            
        case 0:
            print("sb ");
            RVsb(cpu);
            break;
            
        case 1:
            print("sh ");
            RVsh(cpu);
            break;
            
        case 2:
            print("sw ");
            RVsw(cpu);
            break;
            
        case 3:
            cpu->mode = RV64I;
            print("sd");
            RVsd(cpu);
            break;
            
        default:
            TRAP(cpu);
            break;
            
    }
    
    
}


void RVlui(RISCV_t* cpu){
    print("%s,%d",abi[cpu->rd],cpu->imm);
    
    cpu->xReg[cpu->rd] = cpu->imm << 12;
    cpu->cycle += 1;
}

void OPCode_0110111(RISCV_t* cpu){
    UMode(cpu);
    print("lui ");
    RVlui(cpu);
}

void OPCode_0110011(RISCV_t* cpu){
    
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
            break;
            
        case 1:
            print("sll ");
            RVsll(cpu);
            break;
            
        case 2:
            print("slt ");
            RVslt(cpu);
            break;
            
        case 3:
            print("sltu ");
            RVsltu(cpu);
            break;
            
        case 4:
            print("xor ");
            RVxor(cpu);
            break;
            
        case 5:
            if( cpu->func7 == 0x20){
                print("sra ");
                RVsra(cpu);
            }else{
                print("srl ");
                RVsrl(cpu);
            }
            break;
            
        case 6:
            print("or ");
            RVor(cpu);
            break;
            
        case 7:
            print("and ");
            RVand(cpu);
            break;
    }
    
    
}


void OPCode_1110011(RISCV_t* cpu){
    
    IMode(cpu);
    
    switch(cpu->imm){
            
        case 0:
            print("ecall ");
            break;
        case 1:
            print("ebreak ");
            break;
    }
    
}




void RVbeq(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] == cpu->xReg[cpu->rs2]){
        
        //cpu->pc += (cpu->imm - 4); //minus 4 because the instruction decoder always adds 4... except the compressed which adds 2, I'll need to account for that when doing the compressed set.
        
    }
    
    cpu->cycle += 1;
    
}

void RVbne(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] != cpu->xReg[cpu->rs2]){
        
        cpu->pc += (cpu->imm - 4); //minus 4 because the instruction decoder always adds 4... except the compressed which adds 2, I'll need to account for that when doing the compressed set.
        
    }
    
    cpu->cycle += 1;
}

void RVblt(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] < cpu->xReg[cpu->rs2]){
        
        //cpu->pc += (cpu->imm - 4); //minus 4 because the instruction decoder always adds 4... except the compressed which adds 2, I'll need to account for that when doing the compressed set.
        
    }
    
    cpu->cycle += 1;
}


void RVbge(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] >= cpu->xReg[cpu->rs2]){
        
        //cpu->pc += (cpu->imm - 4); //minus 4 because the instruction decoder always adds 4... except the compressed which adds 2, I'll need to account for that when doing the compressed set.
        
    }
    
    cpu->cycle += 1;
}

void RVbltu(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] < cpu->xReg[cpu->rs2]){
        
        //cpu->pc += (cpu->imm - 4); //minus 4 because the instruction decoder always adds 4... except the compressed which adds 2, I'll need to account for that when doing the compressed set.
        
    }
    
    cpu->cycle += 1;
    
}

void RVbgeu(RISCV_t* cpu){
    print("%s, %s, %d",abi[cpu->rs1],abi[cpu->rs2],cpu->imm + cpu->pc);
    
    if(cpu->xReg[cpu->rs1] >= cpu->xReg[cpu->rs2]){
        
        //cpu->pc += (cpu->imm - 4); //minus 4 because the instruction decoder always adds 4... except the compressed which adds 2, I'll need to account for that when doing the compressed set.
        
    }
    
    cpu->cycle += 1;
}

void OPCode_1100011(RISCV_t* cpu){
    
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
            print("blt ");
            RVblt(cpu);
            break;
        case 3:
            print("illegal instruction ");
            break;
        case 4:
            print("blt ");
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


void interruptRequest(void){
    
}

void initRISCV(RISCV_t* cpu){
    

    cpu->cycle = 0;
    cpu->pc = 0;
    cpu->interruptRequest = interruptRequest;
    
    
    //Trap any unimplemented OPCodes
    for(int i=0; i<128;++i){
        cpu->opCode[i] = TRAP;
    }
    
    
    //Build CPU OP Code table
    
    //RV32C set
    cpu->opCode[0] = OPCode_00;
    cpu->opCode[1] = OPCode_01;
    cpu->opCode[2] = OPCode_10;
    
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
    
    
    //FAKE CODEs
    //CPUCore.opCode[67] = doNothing;
    //CPUCore.opCode[83] = doSomething;
    //CPUCore.opCode[91] = doSomethingElse;
    //CPUCore.opCode[107] = resetPC;
    

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
    
    /*
    if((cpu->currentInstruction & 127) == 63){
        
        //64bit length instruction
        TRAP(cpu);
        
    }else */
        
    if((cpu->currentInstruction & 3) == 3){
        cpu->mode = RV32I;
        //32bit Mode
        cpu->opCode[cpu->currentInstruction & 127](cpu);
        cpu->pc += 4;

    }else{
        
        cpu->mode = RV32C;
        //16bit mode
        CompressedMode(cpu);
        cpu->opCode[cpu->currentInstruction & 3](cpu);
        cpu->pc += 2;
        
    }

    //Set xReg0 to 0 after every instruction, to discard any data stored there.
    cpu->xReg[0] = 0;

    
    print("\n");
}



