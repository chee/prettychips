/*     Copyright 2009 chee rabbits ::
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

void chrootnote();
void makeNotes();
void doKeys();

void doWD(int wd);
void doPAN(int pan);
void doVOLUME(int volume);

bool withIn(int f, int m, int l);
void clearanarray(u8 *l, int max);

void touch_me(int sound_id);
void hold_me(int sound_id);
void release_me(int sound_id);
void drone_me(int sound_id);
void play_with_everyone();
void PANIC();

#define NOTEA 440
#define TWELFTHROOT 1.0594630943593
extern enum Scale scale;
enum Scale { Major, Minor, Melodic, Phrygian, Blues };
