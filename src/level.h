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
    Uint16 aboveAddr;
    Uint16 belowAddr;
    Uint16 leftAddr;
    Uint16 rightAddr;
} ScreenData;

void Level_GetScreenMetatiles(int level, int screenX, int screenY);
