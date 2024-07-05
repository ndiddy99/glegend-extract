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
        out->aboveAddr = addr;
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
        out->belowAddr = addr;
    }
}

void Level_GetScreenMetatiles(int level, int screenX, int screenY) {
    ScreenData data;
    Uint8 out[0xc0];

    Level_GetScreenData(screenX, screenY, &data);
    // TODO flesh this out (see $A749)
    Uint16 addr = ROM_Read16(0xa7b0 + (level * 2));
    int cursor = 0;

    Uint8 floorSectionSize = ROM_Read(addr + cursor++);
    Uint8 startColumn = ROM_Read(addr + cursor++);
    Uint8 floorPatternWidth = ROM_Read(addr + cursor++);
    Uint8 floorPatternHeight = ROM_Read(addr + cursor++);
    int floorDataCursor = cursor;

    for (int y = 0; y < 12; y++) {
        for (int x = startColumn; x < 16; x++) {
            int xOffset = x % floorPatternWidth;
            int yOffset = y % floorPatternHeight;
            Uint8 metatile = ROM_Read(addr + floorDataCursor + (yOffset * floorPatternWidth) + xOffset);
            out[y * 16 + x] = metatile;
            printf("%02x ", metatile);
        }
        printf("\n");
    }
}
