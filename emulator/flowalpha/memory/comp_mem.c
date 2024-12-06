/*
 comp_mem.c
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Memory utility functions for the emulator
 Copyright (c) 2024 Maxims Enterprise
*/

#include "emulator.h"
#include <_stdio.h>
#include <stdio.h>

int fetch(int address) {
    if (address < 0x10000) {
        return ram[address];
    } else if (address > 0xFFFF && address <= 0xFFFFF) {
        return rom[address - 0x10000];
    } else {
        printf("Invalid address 0x%X\n", address);
        return -1;
    }
}

void set(int address, int value) {
    if (value < 0 || value > 255) {
        return;
    }
    if (address < 0x10000) {
        ram[address] = value;
    } else if (address > 0xFFFF && address <= 0xFFFFF) {
        rom[address - 0x10000] = value;
    }
}

void write_rom() {
    FILE *file = fopen(path, "wb");

    if (file == NULL) {
        printf("Could not open file\n");
        return;
    }

    fwrite(rom, 1, 960 * 1024, file);
    fclose(file);
}

void set_reg(int reg, int value) {
    if (reg < 0 || reg > 7) {
        return;
    }
    registers[reg] = value;
}

int get_stack_pointer() { return ram[0xFFEB]; }

int push(int value) {
    int overflow = 0;
    int sp = get_stack_pointer();
    if (sp == 0) {
        overflow = 1;
    } else {
        ram[sp] = value;
        set(0xFFEB, sp - 1);
    }
    return overflow;
}

int pop() {
    int sp = get_stack_pointer();
    if (sp == 0xFF) {
        return -1;
    }
    set(0xFFEB, sp + 1);
    return ram[sp + 1];
}
