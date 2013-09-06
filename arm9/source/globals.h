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

u16 notes[13];
u16 keys[8];
u16 root;

int scale;
int octave;
int curkey;

//Rhythm.
int bpm;
int hpm;
int lpm;
//for setting a PSG
int ids[8];
int wCycle;
u8 nVolume;
u8 nPan;

//other
int cscreen;
bool trem_on;
bool rel_on;

bool noteHeld[8];


int volbent;
float pitching;
float ppeak;
u8 vpeak;
float whammy;


int trem_speed;
u8 rlength;
int bg3;
int bg3sub;
int killme[8];
u8 myVolume[8];
u8 myPan[8];


bool wOval;
bool fTri;
int wovalate;
int ftriangulate;

float noted[8];

int drone;