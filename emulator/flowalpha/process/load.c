/*
 load.c
 As part of the Flow Alpha project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Load capabilities for files
 Copyright (c) 2024 Maxims Enterprise
*/

#include "emulator.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void load(const char *fch_path) {
    FILE *file = fopen(fch_path, "rb");
    if (file == NULL) {
        printf("Could not open file\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *buffer = malloc(fsize);
    if (buffer == NULL) {
        perror("Could not allocate memory");
        fclose(file);
        return;
    }

    size_t bytes_read = fread(buffer, 1, fsize, file);
    if (bytes_read != fsize) {
        perror("Could not read file");
        free(buffer);
        fclose(file);
        return;
    }

    fclose(file);

    int origin = 0;
    if (buffer[0] == 0x2A) {
        origin = (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
        for (size_t i = 4; i < fsize && i < sizeof(rom); i++) {
            set(origin, buffer[i]);
            origin++;
        }
    } else {
        printf("Invalid file format. Origin could not be found\n");
    }
}
