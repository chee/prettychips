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

#include "menus.h"
#include "extras.h"
#include "globals.h"
#include "menu1.h"
#include "menu2.h"
#include "menu3.h"
#include "menu5.h"
#include "mytimers.h"
#include <nds.h>

int menuon;

typedef struct {
  int fx;
  int lx;
  int fy;
  int ly;
} touchPlace;

int cscreen;

// make buttons.
touchPlace majorb; // first screen
touchPlace minorb;
touchPlace melminb;
touchPlace phrygb;
touchPlace bluesb;

touchPlace octdb;
touchPlace octub;

touchPlace keyab;
touchPlace keyasb;
touchPlace keybb;
touchPlace keycb;
touchPlace keycsb;
touchPlace keydb;
touchPlace keydsb;
touchPlace keyeb;
touchPlace keyfb;
touchPlace keyfsb;
touchPlace keygb;
touchPlace keygsb;

touchPlace wdab; // second screen
touchPlace wdbb;
touchPlace wdcb;
touchPlace wddb;
touchPlace wdeb;
touchPlace wdfb;

touchPlace trem; // third screen
touchPlace pRamp;
touchPlace vRamp;
touchPlace aTack;
touchPlace rLease;

void makeMeSomeButtons() {
  // define buttons.
  majorb.fx = 8; // first screen
  majorb.lx = 46;
  minorb.fx = 58;
  minorb.lx = 96;
  melminb.fx = 108;
  melminb.lx = 146;
  phrygb.fx = 158;
  phrygb.lx = 196;
  bluesb.fx = 208;
  bluesb.lx = 246;

  octdb.fx = 80;
  octdb.lx = 118;
  octub.fx = 136;
  octub.lx = 174;

  keyab.fx = 6;
  keyab.lx = 22;
  keyasb.fx = 23;
  keyasb.lx = 50;
  keybb.fx = 51;
  keybb.lx = 66;
  keycb.fx = 67;
  keycb.lx = 82;
  keycsb.fx = 83;
  keycsb.lx = 110;
  keydb.fx = 111;
  keydb.lx = 124;
  keydsb.fx = 125;
  keydsb.lx = 150;
  keyeb.fx = 151;
  keyeb.lx = 164;
  keyfb.fx = 165;
  keyfb.lx = 176;
  keyfsb.fx = 177;
  keyfsb.lx = 201;
  keygb.fx = 202;
  keygb.lx = 217;
  keygsb.fx = 218;
  keygsb.lx = 248;

  wdab.fx = 2; // second screen;
  wdab.lx = 39;
  wdbb.fx = 45;
  wdbb.lx = 82;
  wdcb.fx = 88;
  wdcb.lx = 125;
  wddb.fx = 130;
  wddb.lx = 167;
  wdeb.fx = 173;
  wdeb.lx = 210;
  wdfb.fx = 216;
  wdfb.lx = 253;

  trem.fy = 45;
  trem.ly = 186;
}

void setMenuUP() {
  switch (cscreen) {
  case 0:
    dmaCopy(menu1Bitmap, bgGetGfxPtr(bg3), 256 * 256);
    dmaCopy(menu1Pal, BG_PALETTE, 256 * 2);
    break;
  case 1:
    dmaCopy(menu2Bitmap, bgGetGfxPtr(bg3), 256 * 256);
    dmaCopy(menu2Pal, BG_PALETTE, 256 * 2);
    break;
  case 2:
    dmaCopy(menu3Bitmap, bgGetGfxPtr(bg3), 256 * 256);
    dmaCopy(menu3Pal, BG_PALETTE, 256 * 2);
    break;
  case 4:
    dmaCopy(menu5Bitmap, bgGetGfxPtr(bg3), 256 * 256);
    dmaCopy(menu5Pal, BG_PALETTE, 256 * 2);
    break;
  }
}

void isWeHasButton(int px, int py) {

  int ra;
  int inde;
  if (withIn(TOPMENUFY, py, TOPMENULY)) {
    if (withIn(1, px, 49)) {
      cscreen = 0;
      setMenuUP();
    }
    if (withIn(52, px, 101)) {
      cscreen = 1;
      setMenuUP();
    }
    if (withIn(103, px, 152)) {
      cscreen = 2;
      setMenuUP();
    }
    if (withIn(208, px, 255)) {
      cscreen = 4;
      setMenuUP();
    }
  }

  if (cscreen == 0) {
    if (withIn(MENU1TFY, py, MENU1TLY)) {
      if (withIn(majorb.fx, px, majorb.lx))
	scale = 0;
      if (withIn(minorb.fx, px, minorb.lx))
	scale = 1;
      if (withIn(melminb.fx, px, melminb.lx))
	scale = 2;
      if (withIn(phrygb.fx, px, phrygb.lx))
	scale = 3;
      if (withIn(bluesb.fx, px, bluesb.lx))
	scale = 4;
      doKeys();
    }

    if (withIn(MENU1BFY, py, MENU1BLY)) {
      if (withIn(keyab.fx, px, keyab.lx))
	curkey = 0;
      if (withIn(keyasb.fx, px, keyasb.lx))
	curkey = 1;
      if (withIn(keybb.fx, px, keybb.lx))
	curkey = 2;
      if (withIn(keycb.fx, px, keycb.lx))
	curkey = 3;
      if (withIn(keycsb.fx, px, keycsb.lx))
	curkey = 4;
      if (withIn(keydb.fx, px, keydb.lx))
	curkey = 5;
      if (withIn(keydsb.fx, px, keydsb.lx))
	curkey = 6;
      if (withIn(keyeb.fx, px, keyeb.lx))
	curkey = 7;
      if (withIn(keyfb.fx, px, keyfb.lx))
	curkey = 8;
      if (withIn(keyfsb.fx, px, keyfsb.lx))
	curkey = 9;
      if (withIn(keygb.fx, px, keygb.lx))
	curkey = 10;
      if (withIn(keygsb.fx, px, keygsb.lx))
	curkey = 11;
      chrootnote();
      makeNotes();
      doKeys();
    }
  } else if (cscreen == 1) {

    if (withIn(MENU2MFY, py, MENU2MLY))
      doPAN(px / 2);

    if (withIn(MENU2BFY, py, MENU2BLY))
      doVOLUME(px / 2);

    if (withIn(MENU2TFY, py, MENU2TLY)) {
      if (withIn(0, px, 41))
	doWD(0);
      if (withIn(43, px, 84))
	doWD(1);
      if (withIn(86, px, 128))
	doWD(2);
      if (withIn(129, px, 169))
	doWD(3);
      if (withIn(171, px, 212))
	doWD(4);
      if (withIn(214, px, 255))
	doWD(5);
    }

  }

  else if (cscreen == 2) {
    if (withIn(45, py, 186)) {
      ra = py - 45;
      inde = ra - 70;
      if (withIn(MWHAMF, px, MWHAML))
	whammy = inde * 4;
      if (withIn(MPITCHF, px, MPITCHL))
	pitching = inde;
      if (withIn(MVOLUMEF, px, MVOLUMEL))
	volbent = inde / 10;
      if (withIn(MPPF, px, MPPL)) {

	if (withIn(45, py, 65))
	  ppeak = 0.1;

	if (withIn(66, py, 86))
	  ppeak = 0.2;

	if (withIn(87, py, 107))
	  ppeak = 0.3;

	if (withIn(108, py, 128))
	  ppeak = 0.5;

	if (withIn(129, py, 149))
	  ppeak = 1;

	if (withIn(150, py, 170))
	  ppeak = 2;

	if (withIn(171, py, 186))
	  ppeak = 4;
      }
    }

  }

  else if (cscreen == 4) {
    if (withIn(33, py, 83)) {
      if (withIn(2, px, 63))
	curkey = 0;
      if (withIn(65, px, 126))
	curkey = 1;
      if (withIn(128, px, 189))
	curkey = 2;
      if (withIn(191, px, 252))
	curkey = 3;
    }

    if (withIn(86, py, 136)) {
      if (withIn(2, px, 63))
	curkey = 4;
      if (withIn(65, px, 126))
	curkey = 5;
      if (withIn(128, px, 189))
	curkey = 6;
      if (withIn(191, px, 252))
	curkey = 7;
    }

    if (withIn(140, py, 190)) {
      if (withIn(2, px, 63))
	curkey = 8;
      if (withIn(65, px, 126))
	curkey = 9;
      if (withIn(128, px, 189))
	curkey = 10;
      if (withIn(191, px, 252))
	curkey = 11;
    }
    chrootnote();
    makeNotes();
    doKeys();
  }
}

/* octave was going HAYWIRE when it was controlled by held,
 * so this is a function for any buttons that can't
 * handle held.  (Octave needed slow incrementation, one
 * per push. */
void keysDownOnly(int px, int py) {
  switch (cscreen) {
  case 0:
    if (withIn(MENU1MFY, py, MENU1MLY)) {
      if (withIn(octdb.fx, px, octdb.lx))
	octave--;

      if (withIn(octub.fx, px, octub.lx))
	if (octave < 7)
	  octave++;

      chrootnote();
      makeNotes();
      doKeys();
      break;
    }
  }
  if (cscreen == 2) {
    if (withIn(208, px, 253)) {
      if (withIn(46, py, 90))
	wOval = !wOval;
      if (withIn(92, py, 136))
	fTri = !fTri;
    }
  }
}