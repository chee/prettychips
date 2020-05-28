/*     Copyright 2009 Graeme Roberts ::
        prettychips is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    prettychips is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with prettychips.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "globals.h"
#include <math.h>
#include <nds.h>

float powfer(float x, float y);
void chrootnote();
void makeNotes();
void doKeys();

void doWD(int wd);
void doPAN(int pan);
void doVOLUME(int volume);

bool withIn(int f, int m, int l);
void clearanarray(u8 *l, int max);

void touchMe(int soundid);
void holdMe(int soundid);
void killMe(int soundid);
void LMe(int soundid);
void PANIC();

#define NOTEA 440
#define TWELFTHROOT 1.0594630943593
