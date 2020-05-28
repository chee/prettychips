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

#include <nds.h>

#define NOTEA 440
#define TWELFTHROOT 1.0594630943593

// For calculating the notes.

extern u16 notes[13];
extern u16 keys[8];
extern u16 root;

extern int scale;
extern int octave;
extern int curkey;

// Rhythm.
extern int bpm;
extern int hpm;
extern int lpm;
// for setting a PSG
extern int ids[8];
extern int wCycle;
extern u8 nVolume;
extern u8 nPan;

// other
extern int cscreen;
extern bool trem_on;
extern bool rel_on;

extern bool noteHeld[8];

extern int volbent;
extern float pitching;
extern float ppeak;
extern u8 vpeak;
extern float whammy;

extern int trem_speed;
extern u8 rlength;
extern int bg3;
extern int bg3sub;
extern int killme[8];
extern u8 myVolume[8];
extern u8 myPan[8];

extern bool wOval;
extern bool fTri;
extern int wovalate;
extern int ftriangulate;

extern float noted[8];

extern int drone;