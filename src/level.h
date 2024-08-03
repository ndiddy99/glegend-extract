/* level.h: Level data utility functions
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

#pragma once
#include "constants.h"

typedef struct {
    Uint16 addr;
    Uint16 topAddr;
    Uint16 bottomAddr;
    Uint16 leftAddr;
    Uint16 rightAddr;
} ScreenData;

typedef struct {
    Uint8 byte_5d;
    Uint8 byte_5e;
    Uint8 byte_57;
    Uint8 byte_55;
} ScreenBlockData;

/**
 * @brief Draws the metatiles for a given screen
 * @param level level number
 * @param screenX screen x coordinate
 * @param screenY screen y coordinate
 * @param out metatile array to write to (must be size 0xc0)
 */
void Level_GetScreenMetatiles(int level, int screenX, int screenY, Uint8 *out);
