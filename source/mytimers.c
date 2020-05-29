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

#include "mytimers.h"
#include "extras.h"
#include "globals.h"
#include <nds.h>
#include <stdio.h>

/*

 *

 *  Future Use

 *

*/

int noisy = 0;

void effect() {
  TIMER1_CR = 1;
  TIMER_DATA(1) = TIMER_FREQ_1024(hpm * 4);
  TIMER1_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
  irqSet(IRQ_TIMER1, effective);
  irqEnable(IRQ_TIMER1);
}
void theBeat() {
  TIMER0_CR = 0;
  TIMER0_DATA = TIMER_FREQ_1024(1);
  TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024 | TIMER_IRQ_REQ;
  irqSet(IRQ_TIMER0, NULL);
  irqEnable(IRQ_TIMER0);
}

void effective() {
  if (lpm != hpm) {
    timerStop(1);
    effect();
  }
  lpm = hpm;
  int i;
  wovalate = !wovalate;
  ftriangulate = !ftriangulate;

  for (i = 0; i < 8; i++) {
    if (noteHeld[i] == true) {

      if (wOval == true)
	soundSetWaveDuty(ids[i], (wCycle + wovalate));
      if (fTri == true)
	noted[i] = keys[i] + (keys[i] * ftriangulate);
    }
  }
}
void effectcallback() {

  /*
  int i;
  if (trem_on == 1)

                  for (i = 0; i < 8; i++) {

                          nVolume += trem_speed;

                          if (nVolume < 4)
                                  trem_speed = -trem_speed;

                          soundSetVolume(ids[i], nVolume);
                  } */
}

// void rel_timer() {
// u8 i = 8;
//	TIMER2_CR = 1;
//	TIMER_DATA(2) = TIMER_FREQ_64(40);
//	TIMER2_CR = TIMER_ENABLE | TIMER_DIV_64 | TIMER_IRQ_REQ;
//	irqSet(IRQ_TIMER2, do_rel);
//	irqEnable(IRQ_TIMER2);
//}

// void do_rel() {

//}
