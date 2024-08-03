/* tile.h: Handles converting metatiles to tiles
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

/**
 * @brief Gets the tiles for a given screen
 * @param areaNum Area number
 * @param metatiles (in) the metatiles for that screen, must be an array of size 0xc0
 * @param tiles (out) the tiles for that screen, must be an array of size 0x300
 * @param pals (out) the palettes for that screen, must be an array of size 0x300
 */
void Tile_GetScreenTiles(int areaNum, Uint8 *metatiles, Uint8 *tiles, Uint8 *pals);
