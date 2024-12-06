/*
 main.c
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Entry point for the simulator
 Copyright (c) 2024 Maxims Enterprise
*/

#include "emulator.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t ram[64 * 1024] = {0};
uint8_t rom[960 * 1024] = {0};
char path[256];

int main(int argc, char *argv[]) {
    // Set stack pointer to FF
    ram[0xFFEB] = 0xFF;
    if (argc == 1) {
        printf("Flow Alpha simulator\n");
        printf("Created by Max Van den Eynde and Maxims Enterprise\n");
        printf("Usage: flowalpha new -> Create a new file\n");
        printf("       flowalpha -r rom.flrom -> Load a ROM file\n");
        return 1;
    } else {
        if (argc == 2) {
            if (strcmp(argv[1], "new") == 0) {
                printf("Creating new Flux Alpha machine\n");
                printf("Please enter the path of the .flrom file for storing "
                       "the ROM\n");

                printf("(Will take up 960KB of space)\n");
                while (true) {
                    printf("> ");

                    scanf("%s", path);

                    if (strlen(path) < 6 ||
                        strcmp(path + strlen(path) - 6, ".flrom") != 0) {
                        printf("Invalid file extension. Please use .flrom\n");
                        continue;
                    }

                    FILE *file = fopen(path, "wb");
                    if (file == NULL) {
                        printf("Could not open file\n");
                        continue;
                    }

                    size_t size = 960 * 1024;

                    unsigned char buffer[1024];
                    for (size_t i = 0; i < 1024; ++i) {
                        buffer[i] = 0x00;
                    }

                    for (size_t written = 0; written < size; written += 1024) {
                        size_t remaining = size - written;
                        size_t to_write = remaining < 1024 ? remaining : 1024;
                        fwrite(buffer, 1, to_write, file);
                    }

                    fclose(file);
                    printf("ROM created successfully\n");
                    break;
                }
                printf("\033[H\033[J");
                start_simulator();
            } else {
                printf("Unknown command. Try 'new' or '-r rom.flrom'\n");
            }
        } else if (argc == 3) {
            if (strcmp(argv[1], "-r") == 0) {
                strncpy(path, argv[2], sizeof(path) - 1);
                path[sizeof(path) - 1] = '\0';
                printf("Loading ROM from %s\n", path);
                load_rom(path);
                printf("\033[H\033[J");
                start_simulator();
            } else {
                printf("Unknown command. Try 'new' or '-r rom.flrom'\n");
            }
        } else if (argc == 5) {
            if (strcmp(argv[1], "-r") == 0) {
                strncpy(path, argv[2], sizeof(path) - 1);
                path[sizeof(path) - 1] = '\0';
                printf("Loading ROM from %s\n", path);
                load_rom(path);
                if (strcmp(argv[3], "-l") == 0) {
                    load(argv[4]);
                }
                start_simulator();
            } else {
                printf("Unknown command. Try 'new' or '-r rom.flrom'\n");
            }
        } else {
            printf("Unknown command. Try 'new' or '-r rom.flrom'\n");
        }
    }
    return 0;
}
