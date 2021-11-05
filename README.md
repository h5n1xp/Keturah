# Keturah
Simple RISC-V Emulator

A RISC-V emulator in the same vain as the 68k emulator Musashi.

It implements the RV32I instruction set, plus a few RV64I instructions (by default the regististers are 64bit wide, but can bet set to 32bit by commenting out the #define R64).

The compressed RV32C instructions are decoded but not fully implemented yet, the entire 16bit ISA  decoding stage is a mess and needs to be rewritten.

