/* rom.c: ROM file handler
*  Copyright (c) 2024 Nathan Misner
*
*  This file is part of glegend-extract.
*
*  glegend-extract is free software: you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the Free
*  Software Foundation; either version 2 of the License, or (at your option)
*  any later version.
*
*  glegend-extract is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
*  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
*  more details.
*
*  You should have received a copy of the GNU General Public License
*  along with glegend-extract. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "rom.h"

#define ROM_SIZE 0x20000
#define HEADER_SIZE 16
#define FILE_SIZE (ROM_SIZE + HEADER_SIZE)
static Uint8 romData[ROM_SIZE];

#define BANK_SIZE 0x4000
#define MIN_BANK 0
#define MAX_BANK 7
#define FIXED_BANK 7
#define MIN_ADDR 0x8000
#define MIN_FIXED_ADDR 0xc000
#define MAX_ADDR 0xffff
static int bank;


int ROM_Init(void) {
    bank = 0;

    FILE *fp = fopen("glegend.nes", "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        // make sure file size matches
        if (ftell(fp) != FILE_SIZE) {
            fclose(fp);
            printf("Invalid ROM size.\n");
            return 0;
        }
        // seek past header
        fseek(fp, HEADER_SIZE, SEEK_SET);
        // read in data
        fread(romData, 1, ROM_SIZE, fp);
        fclose(fp);
        return 1;
    }

    printf("Error opening ROM file.\n");
    return 0;
}

void ROM_SetBank(int newBank) {
    if ((newBank < MIN_BANK) || (newBank > MAX_BANK)) {
        printf("Invalid bank number %d\n", newBank);
        abort();
    }
    bank = newBank;
}

Uint8 ROM_Read(Uint16 addr) {
    if ((addr < MIN_ADDR) || (addr > MAX_ADDR)) {
        printf("Invalid read address %d\n", addr);
        abort();
    }

    if (addr >= MIN_FIXED_ADDR) {
        return romData[(FIXED_BANK * BANK_SIZE) + (addr - MIN_FIXED_ADDR)];
    }
    else {
        return romData[(bank * BANK_SIZE) + (addr - MIN_ADDR)];
    }
}

Uint16 ROM_Read16(Uint16 addr) {
    return ROM_Read(addr) | (ROM_Read(addr + 1) << 8);
}
