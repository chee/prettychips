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
#include "extras.h"
#include <nds.h>



float powfer(float x, float y) {
	float final;
	final = x;
	int i;
for (i = 1; i < y; i++)

	x *= final;

	return x;
}

void chroot() {
	int i;
	root = NOTEA/2;
	for (i = 0; i < octave; i++)
		root += root;

	root *= (powfer(TWELFTHROOT, (curkey + 1)));
	root -= TWELFTHROOT;
	root -= TWELFTHROOT;
}

void makeNotes() {
	int i;
	for (i = 0; i < 13; i++)
	{
		notes[i] = root * (powfer(TWELFTHROOT, (i+1) ) );
	}
}

void doKeys() {
	switch (scale) {

		default:  // Major Scale
			keys[0] = notes[0];
			keys[1] = notes[2];
			keys[2] = notes[4];
			keys[3] = notes[5];
			keys[4] = notes[7];
			keys[5] = notes[9];
			keys[6] = notes[11];
			keys[7] = notes[12];
			break;

		case 1:  // Minor Scale
			keys[0] = notes[0];
			keys[1] = notes[2];
			keys[2] = notes[3];
			keys[3] = notes[5];
			keys[4] = notes[7];
			keys[5] = notes[8];
			keys[6] = notes[10];
			keys[7] = notes[12];
			break;

		case 2:  // Mel. Minor Scale
			keys[0] = notes[0];
			keys[1] = notes[2];
			keys[2] = notes[3];
			keys[3] = notes[5];
			keys[4] = notes[7];
			keys[5] = notes[9];
			keys[6] = notes[11];
			keys[7] = notes[12];
			break;

		case 3:  // Phrygian Scale
			keys[0] = notes[0];
			keys[1] = notes[1];
			keys[2] = notes[3];
			keys[3] = notes[5];
			keys[4] = notes[7];
			keys[5] = notes[8];
			keys[6] = notes[10];
			keys[7] = notes[12];
			break;

		case 4:  // Blues Scale
			keys[0] = notes[0];
			keys[1] = notes[3];
			keys[2] = notes[5];
			keys[3] = notes[6];
			keys[4] = notes[7];
			keys[5] = notes[10];
			keys[6] = notes[11];
			keys[7] = notes[12];
			break;
	}
}

bool withIn(int f, int m, int l) {
	if (f < m && m < l)
		return true;
	else if (f > m || m > l)
		return false;
return 0;
}

void doWD(int wd) {
	int i;
	for (i = 0; i < 8; i++)
		soundSetWaveDuty(ids[i], wd);
	wCycle = wd;
}

void doPAN(int pan) {
	int i;
	for (i = 0; i < 8; i++)
		soundSetPan(ids[i], pan);
	nPan = pan;
}


void doVOLUME(int volume) {
	nVolume = volume;
}

void clearanarray(u8 *l, int max) {
	int i;
for (i = 0; i < max; i++)
l[i] = 0;
}
int v;
float p;

void touchMe(int soundid) {
int i;
	if (drone > 4) {
		for (i=0;i<8;i++)
			if (noteHeld[i] == false)
				soundKill((i+8)); 
		drone = 0;
	}
	p = pitching;
	v = volbent;

	noteHeld[soundid] = true;
	myVolume[soundid] = nVolume;
	ids[soundid] = soundPlayPSG(wCycle, keys[soundid], myVolume[soundid], nPan);
	noted[soundid] = keys[soundid];
}

void holdMe(int soundid) {
	noteHeld[soundid] = true;

	if (myVolume[soundid] + v > 126) 
		soundSetVolume(ids[soundid], myVolume[soundid]);
	else 
		soundSetVolume(ids[soundid], myVolume[soundid] + v);
	
	soundSetPan(ids[soundid], myPan[soundid]);
		
	if (keys[soundid] + p + whammy > keys[soundid]* (2*ppeak) || keys[soundid] + p + whammy < keys[soundid] / (2*ppeak))
		p = 0;

	soundSetFreq(ids[soundid], noted[soundid] + p + whammy);
	v += volbent;
	p += pitching;
	myVolume[soundid] = nVolume;
	myPan[soundid] = nPan;
}


void killMe(int soundid) {
	p = 0;
	v = 0;
	soundKill(ids[soundid]);
	noteHeld[soundid] = false;
}

// drones.
void LMe(int soundid) {
	p = 0;
	v = 0;
	noteHeld[soundid] = false;
	drone++;
}

// Panic button, kills every PSG channel (including noise channels);
void PANIC() {
	int i;
	for (i = 8; i < 16; i++)
		soundKill(i);
}
