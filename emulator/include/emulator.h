/*
 emulator.h
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Emulator headers
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

uint8_t rom[960 * 1024];
uint8_t ram[64 * 1024];
uint8_t registers[8];
char path[256];
int ZF;
int CF;
int SF;
int OF;

void start_simulator();
void load_rom(const char *path);
void debug_console(int _);

int fetch(int address);
void set(int address, int value);
void set_reg(int reg, int value);
void write_rom();

void load(const char *fch_path);
void execute(int address);

int get_stack_pointer();
int push(int value);
int pop();

#endif
