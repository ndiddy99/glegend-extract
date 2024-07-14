/* main.c: Entry point
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
#include "level.h"
#include "rom.h"

int main(int argc, char **argv) {
    if (!ROM_Init()) {
        return -1;
    }

    Level_GetScreenMetatiles(0, 12, 12);

    return 0;
}