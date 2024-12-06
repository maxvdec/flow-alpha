/*
 execute.c
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Main execution loop
 Copyright (c) 2024 Maxims Enterprise
*/

#include "emulator.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int ZF = 0;
int CF = 0;
int SF = 0;
int OF = 0;

void execute(int address) {
    int halt = 0;
    int current_region = address;
    uint8_t program_counter[3] = {0};

    while (true) {
        for (size_t i = 0; i < 3; i++) {
            program_counter[3 - 1 - i] = (current_region >> (i * 8)) & 0xFF;
        }

        set(0x0FFEB, program_counter[0]);
        set(0x0FFEC, program_counter[1]);
        set(0x0FFED, program_counter[2]);

        int instruction = fetch(current_region);
        switch (instruction) {
        case 0x00:
            printf(
                "There is no operation to run at 0x%05X. Halting processor\n",
                current_region);
            halt = 1;
            break;
        case 0x1D: {
            // 'mov' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] = val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] = fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] = registers[reg2];
                current_region += 2;
            }
            break;
        }
        case 0x11: {
            // 'sub' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] -= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] -= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] -= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x21: {
            // 'jnz' instruction
            if (!ZF) {
                int addr = fetch(current_region + 2) << 16 |
                           fetch(current_region + 1) << 8 |
                           fetch(current_region + 3);
                current_region = addr - 1;
            }
            break;
        }
        case 0x20:
            // 'jmp' instruction
            int addr = fetch(current_region + 2) << 16 |
                       fetch(current_region + 1) << 8 |
                       fetch(current_region + 3);
            current_region = addr - 1;
        case 0x24:
            // 'jz' instruction
            if (ZF) {
                int addr = fetch(current_region + 2) << 16 |
                           fetch(current_region + 1) << 8 |
                           fetch(current_region + 3);
                current_region = addr - 1;
            }
            break;
        case 0x25:
            // 'jg' instruction
            if (!ZF && !SF) {
                int addr = fetch(current_region + 2) << 16 |
                           fetch(current_region + 1) << 8 |
                           fetch(current_region + 3);
                current_region = addr - 1;
            }
            break;
        case 0x26:
            // 'jl' instruction
            if (!ZF && SF) {
                int addr = fetch(current_region + 2) << 16 |
                           fetch(current_region + 1) << 8 |
                           fetch(current_region + 3);
                current_region = addr - 1;
            }
            break;
        case 0x27:
            while (1) {
            }
        case 0x29:
            continue;
        case 0x10: {
            // 'add' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] += val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] += fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] += registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x12: {
            // 'mul' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] *= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] *= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] *= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x13: {
            // 'div' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] /= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] /= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] /= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x14:
            // 'inc' instruction
            registers[fetch(current_region + 1)]++;
            current_region++;
            break;
        case 0x15:
            // 'dec' instruction
            registers[fetch(current_region + 1)]--;
            current_region++;
            break;
        case 0x16:
            // 'switch' instruction
            registers[fetch(current_region + 1)] *= -1;
            current_region++;
        case 0x17: {
            // 'and' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] &= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] &= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] &= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x18: {
            // 'or' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] |= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] |= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] |= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x19: {
            // 'xor' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] ^= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] ^= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] ^= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x1A:
            // 'not' instruction
            registers[fetch(current_region + 1)] =
                ~registers[fetch(current_region + 1)];
        case 0x1B: {
            // 'shiftl' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] <<= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] <<= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] <<= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x1C: {
            // 'shiftr' instruction
            int reg = fetch(current_region + 1);
            int val;
            if (fetch(current_region + 2) == 0xFD) {
                val = fetch(current_region + 3);
                registers[reg] >>= val;
                current_region += 3;
            } else if (fetch(current_region + 2) == 0xFF) {
                val = fetch(current_region + 3);
                registers[reg] >>= fetch(val);
                current_region += 3;
            } else {
                int reg2 = fetch(current_region + 2);
                registers[reg] >>= registers[reg2];
                current_region += 2;
            }
            if (registers[reg] < 0) {
                SF = 1;
            } else if (registers[reg] == 0) {
                ZF = 1;
            } else {
                SF = 0;
                ZF = 0;
            }
            break;
        }
        case 0x1E: {
            // 'push' instruction
            int reg = fetch(current_region + 1);
            int overflow = push(registers[reg]);
            if (overflow) {
                OF = 1;
            }
        }
        case 0x1F: {
            // 'pop' instruction
            int reg = fetch(current_region + 1);
            registers[reg] = pop();
        }
        case 0x22:
            // 'in' instruction
            // Not implemented
        case 0x23:
            // 'out' instruction
            // Not implemented
        default:
            printf("Invalid instruction: 0x%02X. Halting processor\n",
                   instruction);
            halt = 1;
            break;
        }
        current_region++;
        if (halt) {
            break;
        }
    }
    while (1) {
    }
}
