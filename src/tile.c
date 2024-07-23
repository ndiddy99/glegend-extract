/* tile.c: Handles converting metatiles to tiles
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

#include "constants.h"
#include "rom.h"

typedef struct {
    Uint8 byte_478[8];
    Uint8 metatileBases[8];
    Uint16 metatilePtrs[8];
} TilesetData;

void Tile_InitTilesetData(int areaNum, TilesetData *out) {
    Uint8 tileset = ROM_Read(0xad91 + (areaNum * 2) + 1);
    // TODO copy rest of init code if necessary (see $aca9)

    Uint16 dataAddr = 0xae24 + (tileset * 2);
    int count = ROM_Read(dataAddr++);
    for (int i = 0; i < count; i++) {
        int index = ROM_Read(dataAddr) & 0x7;
        out->byte_478[index] = ROM_Read(dataAddr++) >> 4;
        out->metatileBases[index] = ROM_Read(dataAddr++);
        out->metatilePtrs[index] = ROM_Read16(dataAddr);
        dataAddr += 2;
    }
}

void Tile_MetatileRowToTiles(TilesetData *data, Uint8 *metatiles, Uint8 *tiles, Uint8 *pals) {
    for (int i = 15; i >= 0; i--) {
        Uint8 metatile = metatiles[i];
        Uint16 ptr = (metatile & 0x1f) * 5;
        Uint8 dataIndex = metatile >> 5;
        ptr += data->metatilePtrs[dataIndex];
        Uint8 base = data->metatileBases[dataIndex];

        int tileCursor = 0;
        // top left tile
        Uint8 tile = ROM_Read(ptr + tileCursor++);
        if (tile == 0xff) { tile = 0x20; }
        else { tile += base; }
        tiles[i * 2] = tile;
        // top right tile
        tile = ROM_Read(ptr + tileCursor++);
        if (tile == 0xff) { tile = 0x20; }
        else { tile += base; }
        tiles[i * 2 + 1] = tile;
        // bottom left tile
        tile = ROM_Read(ptr + tileCursor++);
        if (tile == 0xff) { tile = 0x20; }
        else { tile += base; }
        tiles[i * 2 + 32] = tile;
        // bottom right tile
        tile = ROM_Read(ptr + tileCursor++);
        if (tile == 0xff) { tile = 0x20; }
        else { tile += base; }
        tiles[i * 2 + 33] = tile;

        Uint8 palette = ROM_Read(ptr + tileCursor++);
        pals[i * 2]      = palette;
        pals[i * 2 + 1]  = palette;
        pals[i * 2 + 32] = palette;
        pals[i * 2 + 33] = palette;
    }
}
