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
#include "extras.h"
#include "adsr.h"
#include <math.h>
#include <nds.h>

bool noteHeld[8];
int ids[8];
int drone;
u16 keys[8];
u8 myPan[8];
u8 volumes[8];
float noted[8];
u16 notes[13];
float ppeak;
u16 root;
enum Scale scale = Major;
ADSR *sound_envelope[8];

float powfer(float x, float y) {
  float final = x;

  for (int i = 1; i < y; i++) {
    x *= final;
  }

  return x;
}

void chrootnote() {
  int i;
  root = NOTEA / 2;
  for (i = 0; i < octave; i++)
    root += root;

  root *= (powfer(TWELFTHROOT, (curkey + 1)));
  root -= TWELFTHROOT;
  root -= TWELFTHROOT;
}

void makeNotes() {
  for (int i = 0; i < 13; i++) {
    notes[i] = root * (powfer(TWELFTHROOT, (i + 1)));
  }
}

void doKeys() {
  switch (scale) {
  case Major:
    keys[0] = notes[0];
    keys[1] = notes[2];
    keys[2] = notes[4];
    keys[3] = notes[5];
    keys[4] = notes[7];
    keys[5] = notes[9];
    keys[6] = notes[11];
    keys[7] = notes[12];
    break;

  case Minor:
    keys[0] = notes[0];
    keys[1] = notes[2];
    keys[2] = notes[3];
    keys[3] = notes[5];
    keys[4] = notes[7];
    keys[5] = notes[8];
    keys[6] = notes[10];
    keys[7] = notes[12];
    break;

  case Melodic: // Mel. Minor Scale
    keys[0] = notes[0];
    keys[1] = notes[2];
    keys[2] = notes[3];
    keys[3] = notes[5];
    keys[4] = notes[7];
    keys[5] = notes[9];
    keys[6] = notes[11];
    keys[7] = notes[12];
    break;

  case Phrygian: // Phrygian Scale
    keys[0] = notes[0];
    keys[1] = notes[1];
    keys[2] = notes[3];
    keys[3] = notes[5];
    keys[4] = notes[7];
    keys[5] = notes[8];
    keys[6] = notes[10];
    keys[7] = notes[12];
    break;

  case Blues: // Blues Scale
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

void doVOLUME(int volume) { nVolume = volume; }

void clearanarray(u8 *l, int max) {
  int i;
  for (i = 0; i < max; i++)
    l[i] = 0;
}
int v;
float p;

u8 get_volume(int sound_id) {
  return (u8)(process(sound_envelope[sound_id]) * 127);
}

void kill_me(int sound_id) {
  soundKill(ids[sound_id]);
  ids[sound_id] = -1;
}

void touch_me(int sound_id) {
  noteHeld[sound_id] = true;

  if (!sound_envelope[sound_id]) {
    ADSR *envelope = malloc(sizeof(ADSR));
    initADSR(envelope);
    sound_envelope[sound_id] = envelope;
    setAttackRate(envelope, 50);
    setDecayRate(envelope, 50);
    setSustainLevel(envelope, 0.5);
    setReleaseRate(envelope, 50);
  }

  resetADSR(sound_envelope[sound_id]);
  sound_envelope[sound_id]->state = env_attack;

  volumes[sound_id] = get_volume(sound_id);

  soundKill(ids[sound_id]);
  ids[sound_id] = soundPlayPSG(wCycle, keys[sound_id], volumes[sound_id], nPan);
  noted[sound_id] = keys[sound_id];
}

void hold_me(int sound_id) { noteHeld[sound_id] = true; }

void release_me(int sound_id) {
  sound_envelope[sound_id]->state = env_release;
  noteHeld[sound_id] = false;
}

void play_with_me(int sound_id) {
  if (ids[sound_id] != -1) {
    volumes[sound_id] = get_volume(sound_id);
    if (sound_envelope[sound_id]->state == env_idle) {
      kill_me(sound_id);
      iprintf("\x1b[%i;0Hshh", sound_id);
    } else {
      soundSetVolume(ids[sound_id], volumes[sound_id]);
      iprintf("\x1b[%i;0H%i", sound_id, volumes[sound_id]);
    }
  }
}

void play_with_everyone() {
  for (int i = 0; i < 8; i++) {
    play_with_me(i);
  }
}

// drones.
void drone_me(int sound_id) {
  p = 0;
  v = 0;
  noteHeld[sound_id] = false;
  drone++;
}

// Panic button, kills every PSG channel (including noise channels);
void PANIC() {
  int i;
  for (i = 8; i < 16; i++)
    soundKill(i);
}
