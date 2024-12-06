/*
 computer.c
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Computer interface and debug panel
 Copyright (c) 2024 Maxims Enterprise
*/

#include "emulator.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t registers[8] = {0};

void start_simulator() {
    signal(SIGINT, debug_console);
    execute(0x10000);
}

void load_rom(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("Could not open file\n");
        return;
    }

    size_t size = 960 * 1024;
    fread(rom, 1, size, file);
    fclose(file);
}

void debug_console(int _) {
    printf("\nDebug console\n");
    printf("Commands:\n");
    printf(" r -> Print registers\n");
    printf(" <register> -> Print register value\n");
    printf(" m <address> -> Print memory value\n");
    printf(" g -> Print general spaces\n");
    printf(" <general space> -> Print general space value\n");
    printf(" set <address | register> <value> -> Set memory value\n");
    printf(" SP -> Print stack pointer\n");
    printf(" s -> Open stepped execution\n");
    printf(" w -> Write ROM to file\n");
    printf(" l <file> -> Load .fch file\n");
    printf(" clear -> Clear screen\n");
    printf(" q -> Quit\n");
    printf(" exit | end -> Exit\n");

    while (true) {
        char command[256];
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "r") == 0) {
            printf("Registers:\n");
            for (int i = 0; i < 8; i++) {
                printf("r%d: 0x%X\n", i, registers[i]);
            }
        } else if (strcmp(command, "clear") == 0) {
            printf("\033[H\033[J");
        } else if (strncmp(command, "m ", 2) == 0) {
            char *rest = &command[2];
            int address = strtol(rest, NULL, 16);
            printf("0x%05X: 0x%02X\n", address, fetch(address));
        } else if (strcmp(command, "SP") == 0) {
            printf("SP: 0x%02X\n", fetch(0xFFEB));
        } else if (strncmp(command, "set", 3) == 0) {
            int address, value;
            if (sscanf(command, "set r%d %X", &address, &value) == 2) {
                set_reg(address, value);
            } else if (sscanf(command, "set %X %X", &address, &value) == 2) {
                set(address, value);
            }
        } else if (command[0] == 'r' && command[1] != '\0') {
            int value = atoi(&command[1]);
            if (value >= 0 && value < 8) {
                printf("r%d: 0x%X\n", value, registers[value]);
            } else {
                printf("Invalid register\n");
            }
        } else if (command[0] == 'l') {
            char *path = &command[2];
            load(path);
        } else if (strcmp(command, "w") == 0) {
            write_rom();
            printf("ROM written to file\n");
        } else if (strcmp(command, "q") == 0) {
            return;
        } else if (strcmp(command, "exit") == 0 ||
                   strcmp(command, "end") == 0) {
            exit(0);
        } else {
            printf("Unknown command\n");
        }
    }
}
