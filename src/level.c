/* level.c: Level data utility functions
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

#include "level.h"
#include "rom.h"

#define MAX_LEVEL_SIZE 24

static void Level_FindScreenDataAddr(int targetX, int targetY, int *xCursor, int *yCursor, Uint16 *addr) {
    while (1) {
        if ((targetX == *xCursor) && (targetY == *yCursor)) {
            return;
        }
        (*addr) += (ROM_Read16(*addr) & 0xf) + 1;
        (*xCursor)++;
        if (*xCursor >= MAX_LEVEL_SIZE) {
            *xCursor = 0;
            (*yCursor)++;
        }
    }
}

void Level_GetScreenData(int screenX, int screenY, ScreenData *out) {
    int xCursor = 0;
    int yCursor = 0;
    Uint16 addr = 0x8a6e;
    ROM_SetBank(5);
    // get data for above the screen
    if (screenY > 0) {
        Level_FindScreenDataAddr(screenX, screenY - 1, &xCursor, &yCursor, &addr);
        out->topAddr = addr;
    }
    // get data for left of screen
    if (screenX > 0) {
        Level_FindScreenDataAddr(screenX - 1, screenY, &xCursor, &yCursor, &addr);
        out->leftAddr = addr;
    }
    // get data for screen
    Level_FindScreenDataAddr(screenX, screenY, &xCursor, &yCursor, &addr);
    out->addr = addr;
    // get data for right of screen
    if (screenX < (MAX_LEVEL_SIZE - 1)) {
        Level_FindScreenDataAddr(screenX + 1, screenY, &xCursor, &yCursor, &addr);
        out->rightAddr = addr;
    }
    // get data for below screen
    if (screenY < (MAX_LEVEL_SIZE - 1)) {
        Level_FindScreenDataAddr(screenX, screenY + 1, &xCursor, &yCursor, &addr);
        out->bottomAddr = addr;
    }
}

void Level_GetScreenBlockData(ScreenData *data, ScreenBlockData *out) {
    int offset = 3;
    out->byte_5d = ROM_Read(data->addr) & 0x60;
    if (out->byte_5d && (out->byte_5d != 0x60)) {
        out->byte_5e = ROM_Read(data->addr + 3);
        offset++;
    }
    else {
        out->byte_5e = 0;
    }
    if (ROM_Read(data->addr + 1) & 0x8) {
        Uint8 val = ROM_Read(data->addr + offset);
        if (val) {
            out->byte_57 = val;
        }
        else {
            out->byte_57 = 1;
        }
    }
    else {
        out->byte_57 = 0;
    }
    out->byte_55 = ROM_Read(0xa377 + out->byte_5e);
}

int Level_CopyMetatileArea(int addr, Uint8 *out) {
    Uint8 offset = ROM_Read(addr++);
    Uint8 width = ROM_Read(addr++);
    Uint8 fill = width & 0x80; width &= 0x7f;
    Uint8 height = ROM_Read(addr++);
    // override address with data from rom
    if (height & 0x80) {
        addr = ROM_Read16(addr);
        addr += 2;
    }
    height &= 0x7f;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            out[(y * 16 + x) + offset] = ROM_Read(addr);
            if (!fill) { addr++; }
        }
    }
}

void Level_LoadCommonMetatiles(int *addr, Uint8 *out) {
    for (int i = 0; i < 4; i++) {
        int length = ROM_Read(*addr);
        if (length) {
            Level_CopyMetatileArea(*addr + 1, out);
            *addr += length;
        }
        else {
            return;
        }
    }
}

void Level_GetScreenMetatiles(int level, int screenX, int screenY) {
    ScreenData data;
    ScreenBlockData blockData;
    Uint8 out[0xc0];

    Level_GetScreenData(screenX, screenY, &data);
    Level_GetScreenBlockData(&data, &blockData);
    // TODO flesh this out (see bank 5 $A749)
    Uint16 addr = ROM_Read16(0xa7b0 + (level * 2)); // $18
    Uint16 addr2 = ROM_Read16(0xa7ca + (level * 2)); // $14
    Uint16 addr3 = 0xa916; // $16
    int cursor = addr;

    // --- draw floor data ---
    Uint8 floorSectionSize = ROM_Read(cursor++);
    Uint8 startColumn = ROM_Read(cursor++);
    Uint8 floorPatternWidth = ROM_Read(cursor++);
    Uint8 floorPatternHeight = ROM_Read(cursor++);
    int floorDataCursor = cursor;
    for (int y = 0; y < 12; y++) {
        for (int x = startColumn; x < 16; x++) {
            int xOffset = x % floorPatternWidth;
            int yOffset = y % floorPatternHeight;
            Uint8 metatile = ROM_Read(floorDataCursor + (yOffset * floorPatternWidth) + xOffset);
            out[y * 16 + x] = metatile;
        }
    }
    cursor = addr + floorSectionSize;

    // --- draw common tiles ---
    Level_LoadCommonMetatiles(&cursor, out);
    cursor = addr2;
    Level_LoadCommonMetatiles(&cursor, out);

    // TODO add check for game mode here (see bank 5 $81BD)
    Uint8 walls = ROM_Read(data.addr + 1);

    // top wall
    if ((ROM_Read(data.topAddr) & 0x80) || (!(walls & 0x80))) {
        Level_CopyMetatileArea(cursor + 1, out);
    }
    cursor += ROM_Read(cursor);

    // left wall
    if ((ROM_Read(data.leftAddr) & 0x80) || !(walls & 0x40)) {
        Level_CopyMetatileArea(cursor + 1, out);
    }
    cursor += ROM_Read(cursor);

    // right wall
    if ((ROM_Read(data.rightAddr) & 0x80) || !(walls & 0x20)) {
        Level_CopyMetatileArea(cursor + 1, out);
    }
    cursor += ROM_Read(cursor);

    // bottom wall
    if ((ROM_Read(data.bottomAddr) & 0x80) || !(walls & 0x10)) {
        Level_CopyMetatileArea(cursor + 1, out);
    }
    cursor += ROM_Read(cursor);

    // think this is only used by shmup mode
    for (int i = 0;; i++) {
        cursor = ROM_Read16(addr3 + (i * 2));
        if (!cursor) { break; }
        Level_CopyMetatileArea(cursor, out);
    }

    // blocks
    if (ROM_Read(data.addr) & 0x10) {
        int blockOffset = 3;
        if (blockData.byte_57) {
            blockOffset++;
        }
        if (blockData.byte_5d && (blockData.byte_5d != 0x60)) {
            blockOffset++;
        }
        addr = ROM_Read16(data.addr + blockOffset);
        Uint8 data = ROM_Read(addr);
        Uint16 blockDataAddr = ((ROM_Read(addr) & 0x1f) * 18) + 0x91ea;
        Uint8 blockDataCursor = 0;
        Uint8 metatileType = ROM_Read(addr + 1);
        Uint8 metatileCursor = 32;
        for (int i = 0; i < 18; i++) {
            Uint8 blockData = ROM_Read(blockDataAddr + blockDataCursor);
            blockDataCursor++;
            for (int j = 0; j < 8; j++) {
                if (blockData & 0x80) {
                    out[metatileCursor] = metatileType;
                }
                blockData <<= 1;
                metatileCursor++;
            }
        }

        Uint8 metatileCount = ROM_Read(addr) >> 5;
        Uint8 dataCursor = 2;
        Uint8 deltaArr[] = { 0xf1, 0x01, 0x10, 0x11 };
        while (metatileCount) {
            metatileType = ROM_Read(addr + dataCursor++);
            Uint8 metatileOffset = ROM_Read(addr + dataCursor++);
            Uint8 metatileAttrs = ROM_Read(addr + dataCursor++);
            Uint8 metatileDelta = deltaArr[(metatileAttrs >> 6) & 3];
            for (int i = 0; i < 6; i++) {
                if (metatileAttrs & 1) {
                    out[metatileOffset] = metatileType;
                }
                metatileAttrs >>= 1;
                metatileOffset += metatileDelta;
            }
            metatileCount--;
        }
    }

    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 16; x++) {
            printf("%02x ", out[y * 16 + x]);
        }
        printf("\n");
    }
}
