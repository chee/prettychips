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

#include "prettychips.h"
#include <math.h>
#include <nds.h>
#include <stdio.h>

float note_a = 440;
float note_as = 466.1637615180899;
float note_b = 493.8833012561241;
float note_c = 261.6255653005987;
float note_cs = 277.18263097687213;
float note_d = 293.66476791740763;
float note_ds = 311.126983722081;
float note_e = 329.62755691287003;
float note_f = 349.228231433004;
float note_fs = 369.9944227116345;
float note_g = 391.99543598174944;
float note_gs = 415.3046975799451;

float octave_0 = -0.0625;
float octave_1 = -0.125;
float octave_2 = 0.25;
float octave_3 = 0.5;
float octave_4 = 1.0;
float octave_5 = 2.0;
float octave_6 = 4.0;
float octave_7 = 8.0;
float octave_8 = 16.0;

PrettyChipsState *state;
Button *buttons[8];
touchPosition touch;

typedef struct {
  int index;
  int x, y;
  u16 *gfx;
  bool active;
} AdsrHandle;

AdsrHandle *attack_handle;
AdsrHandle *decay_sustain_handle;
AdsrHandle *release_handle;

bool xy_is_inside_handle(AdsrHandle *handle, int x, int y) {
  return x > handle->x && x < handle->x + 16 && y > handle->y &&
         y < handle->y + 16;
}

void setup_video() {
  lcdMainOnBottom();
  consoleDemoInit();
  videoSetMode(MODE_0_2D);
  vramSetBankA(VRAM_A_MAIN_SPRITE);
  oamInit(&oamMain, SpriteMapping_1D_32, false);

  attack_handle = malloc(sizeof(AdsrHandle));
  decay_sustain_handle = malloc(sizeof(AdsrHandle));
  release_handle = malloc(sizeof(AdsrHandle));

  attack_handle->index = 0;
  decay_sustain_handle->index = 1;
  release_handle->index = 2;

  attack_handle->active = false;
  decay_sustain_handle->active = false;
  release_handle->active = false;

  attack_handle->gfx =
      oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
  decay_sustain_handle->gfx =
      oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
  release_handle->gfx =
      oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);

  for (int i = 0; i < 16 * 16 / 2; i++) {
    attack_handle->gfx[i] = 1 | (1 << 8);
    decay_sustain_handle->gfx[i] = 1 | (1 << 8);
    release_handle->gfx[i] = 1 | (1 << 8);
  }

  SPRITE_PALETTE[1] = RGB15(3, 21, 31);

  // vramSetBankF(VRAM_F_LCD);

  // VRAM_F_EXT_SPR_PALETTE[0][1] = RGB15(0, 0, 31);
  // VRAM_F_EXT_SPR_PALETTE[1][1] = RGB15(0, 31, 0);
  // VRAM_F_EXT_SPR_PALETTE[2][1] = RGB15(31, 0, 0);

  // vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
}

void update_buttons_scale() {
  switch (state->scale) {
  case scale_major:
    buttons[0]->distance_from_root = 0;
    buttons[1]->distance_from_root = 2;
    buttons[2]->distance_from_root = 4;
    buttons[3]->distance_from_root = 5;
    buttons[4]->distance_from_root = 7;
    buttons[5]->distance_from_root = 9;
    buttons[6]->distance_from_root = 11;
    buttons[7]->distance_from_root = 12;
    break;

  case scale_minor:
    buttons[0]->distance_from_root = 0;
    buttons[1]->distance_from_root = 2;
    buttons[2]->distance_from_root = 3;
    buttons[3]->distance_from_root = 5;
    buttons[4]->distance_from_root = 7;
    buttons[5]->distance_from_root = 8;
    buttons[6]->distance_from_root = 10;
    buttons[7]->distance_from_root = 12;
    break;

  case scale_melodic: // Mel. Minor Scale
    buttons[0]->distance_from_root = 0;
    buttons[1]->distance_from_root = 2;
    buttons[2]->distance_from_root = 3;
    buttons[3]->distance_from_root = 5;
    buttons[4]->distance_from_root = 7;
    buttons[5]->distance_from_root = 9;
    buttons[6]->distance_from_root = 11;
    buttons[7]->distance_from_root = 12;
    break;

  case scale_phrygian: // Phrygian Scale
    buttons[0]->distance_from_root = 0;
    buttons[1]->distance_from_root = 1;
    buttons[2]->distance_from_root = 3;
    buttons[3]->distance_from_root = 5;
    buttons[4]->distance_from_root = 7;
    buttons[5]->distance_from_root = 8;
    buttons[6]->distance_from_root = 10;
    buttons[7]->distance_from_root = 12;
    break;

  case scale_blues: // Blues Scale
    buttons[0]->distance_from_root = 0;
    buttons[1]->distance_from_root = 3;
    buttons[2]->distance_from_root = 5;
    buttons[3]->distance_from_root = 6;
    buttons[4]->distance_from_root = 7;
    buttons[5]->distance_from_root = 10;
    buttons[6]->distance_from_root = 11;
    buttons[7]->distance_from_root = 12;
    break;
  }
}

void update_buttons_adsr() {
  for (u8 i = 0; i < 8; i++) {
    setAttackRate(buttons[i]->envelope, state->attack_rate);
    setDecayRate(buttons[i]->envelope, state->decay_rate);
    setReleaseRate(buttons[i]->envelope, state->release_rate);
    setSustainLevel(buttons[i]->envelope, state->sustain_level);
  }
}

void setup_buttons() {
  for (u8 i = 0; i < 8; i++) {
    buttons[i] = malloc(sizeof(Button));
    Button *button = buttons[i];
    button->envelope = malloc(sizeof(ADSR));
    initADSR(button->envelope);
  }
  buttons[0]->button = KEY_DOWN;
  buttons[1]->button = KEY_LEFT;
  buttons[2]->button = KEY_UP;
  buttons[3]->button = KEY_RIGHT;
  buttons[4]->button = KEY_B;
  buttons[5]->button = KEY_Y;
  buttons[6]->button = KEY_X;
  buttons[7]->button = KEY_A;
  update_buttons_scale();
  update_buttons_adsr();
}

u16 get_button_frequency(Button *button) {
  float root_note = state->key * state->octave;
  float button_note = root_note;
  for (int i = 0; i < button->distance_from_root; i++) {
    button_note = button_note * pow(2.0, 1.0 / 12.0);
  }
  return (u16)button_note;
}

u8 get_volume(ADSR *envelope) {
  float volume = process(envelope);
  return (u8)(volume * 127);
}

void process_notes() {
  for (u8 i = 0; i < 8; i++) {
    Button *button = buttons[i];
    if (button->envelope->state == env_idle) {
      soundKill(button->sound_id);
      resetADSR(button->envelope);
      button->sound_id = -1;
    } else {
      u8 volume = get_volume(button->envelope);
      soundSetVolume(button->sound_id, volume);
    }
  }
}

void attack(KEYPAD_BITS bits) {
  for (u8 i = 0; i < 8; i++) {
    Button *button = buttons[i];
    if (bits & button->button) {
      if (button->envelope->state != env_idle) {
        soundKill(button->sound_id);
        resetADSR(button->envelope);
        button->sound_id = -1;
      }
      button->envelope->state = env_attack;
      u8 volume = get_volume(button->envelope);
      u16 hz = get_button_frequency(button);
      iprintf("\x1b[%i;0Hattack (%i, %i)", i, hz, (u8)(volume * 127));
      button->sound_id = soundPlayPSG(state->wave_duty, hz, 0, 64);
    }
  }
}

void release(KEYPAD_BITS bits) {
  for (u8 i = 0; i < 8; i++) {
    Button *button = buttons[i];
    if (bits & button->button) {
      iprintf("\x1b[%i;0Hrelease", i);
      button->envelope->state = env_release;
    }
  }
}

void setup_initial_state() {
  state = malloc(sizeof(PrettyChipsState));
  state->octave = octave_6;
  state->key = note_c;
  state->scale = scale_blues;
  state->wave_duty = DutyCycle_12;

  state->attack_rate = 100.0;
  state->decay_rate = 100.0;
  state->release_rate = 100.0;
  state->sustain_level = 0.2;
}

void update_handles() {
  attack_handle->y = 0;
  attack_handle->x = (int)((state->attack_rate / 1000.0) * 128.0);

  decay_sustain_handle->x = 128 + ((state->decay_rate / 1000) * 64);
  decay_sustain_handle->y = state->sustain_level * 192;

  release_handle->x = (int)(192.0 + (state->release_rate / 1000.0) * 64.0);
  release_handle->y = 192 - 16;
}

void draw_handle(AdsrHandle *handle) {
  oamSet(&oamMain,      // main graphics engine context
         handle->index, // oam index (0 to 127)
         handle->x,
         handle->y, // x and y pixle location of the sprite
         0,         // priority, lower renders last (on top)
         0, // this is the palette index if multiple palettes or the alpha
         SpriteSize_16x16, SpriteColorFormat_16Color,
         handle->gfx,  // pointer to the loaded graphics
         -1,           // sprite rotation data
         false,        // double the size when rotating?
         false,        // hide the sprite?
         false, false, // vflip, hflip
         false         // apply mosaic
  );
}

void draw_adsr() {
  draw_handle(attack_handle);
  draw_handle(decay_sustain_handle);
  draw_handle(release_handle);
}

int main(void) {
  setup_initial_state();

  soundEnable();
  setup_video();
  setup_buttons();

  while (true) { // main loop
    scanKeys();

    attack(keysDown());
    release(keysUp());
    process_notes();

    touchRead(&touch);

    if (keysDown() & KEY_SELECT & KEY_L) {
      switch (state->scale) {
      case scale_major:
        state->scale = scale_minor;
        break;
      case scale_minor:
        state->scale = scale_melodic;
        break;
      case scale_melodic:
        state->scale = scale_phrygian;
        break;
      case scale_phrygian:
        state->scale = scale_blues;
        break;
      case scale_blues:
        state->scale = scale_major;
        break;
      }
    } else if (keysDown() & KEY_SELECT & KEY_R) {
      switch (state->scale) {
      case scale_major:
        state->scale = scale_blues;
        break;
      case scale_minor:
        state->scale = scale_major;
        break;
      case scale_melodic:
        state->scale = scale_minor;
        break;
      case scale_phrygian:
        state->scale = scale_melodic;
        break;
      case scale_blues:
        state->scale = scale_phrygian;
        break;
      }
    } else if (keysDown() & KEY_R) {
      if (state->octave == octave_0) {
        state->octave = octave_1;
      } else if (state->octave == octave_1) {
        state->octave = octave_2;
      } else if (state->octave == octave_2) {
        state->octave = octave_3;
      } else if (state->octave == octave_3) {
        state->octave = octave_4;
      } else if (state->octave == octave_4) {
        state->octave = octave_5;
      } else if (state->octave == octave_5) {
        state->octave = octave_6;
      } else if (state->octave == octave_6) {
        state->octave = octave_7;
      } else if (state->octave == octave_7) {
        state->octave = octave_8;
      }
    } else if (keysDown() & KEY_L) {
      if (state->octave == octave_2) {
        state->octave = octave_1;
      } else if (state->octave == octave_3) {
        state->octave = octave_2;
      } else if (state->octave == octave_4) {
        state->octave = octave_3;
      } else if (state->octave == octave_5) {
        state->octave = octave_4;
      } else if (state->octave == octave_6) {
        state->octave = octave_5;
      } else if (state->octave == octave_7) {
        state->octave = octave_6;
      } else if (state->octave == octave_7) {
        state->octave = octave_6;
      } else if (state->octave == octave_1) {
        state->octave = octave_0;
      }
    }

    if (keysDown() & KEY_TOUCH) {
      if (xy_is_inside_handle(attack_handle, touch.px, touch.py)) {
        iprintf("we are inside attack");
        attack_handle->active = true;
      }
      if (xy_is_inside_handle(decay_sustain_handle, touch.px, touch.py)) {
        iprintf("we are inside sus");
        decay_sustain_handle->active = true;
      }
      if (xy_is_inside_handle(release_handle, touch.px, touch.py)) {
        // iprintf("we are inside rel");
        release_handle->active = true;
      }
    }
    if (keysHeld() & KEY_TOUCH) {
      if (attack_handle->active == true) {
        float new_rate = (float)touch.px * (1000.0 / 128.0);
        state->attack_rate = new_rate > 900.0 ? 900.0 : new_rate;
      }

      if (decay_sustain_handle->active == true) {
        float new_decay = ((float)touch.px - 128.0) * (1000.0 / 64.0);
        float new_sustain = (float)touch.py * (1.0 / 192.0);
        state->sustain_level = new_sustain;
        state->decay_rate = new_decay;
      }

      if (release_handle->active == true) {
        float new_rate = ((float)touch.px - 192.0) * (900.0 / 64.0);
        state->release_rate = new_rate < 0 ? 0 : new_rate;
      }
    }
    if (keysUp() & KEY_TOUCH) {
      attack_handle->active = false;
      decay_sustain_handle->active = false;
      release_handle->active = false;
    }
    update_handles();
    update_buttons_adsr();

    draw_adsr();

    swiWaitForVBlank();

    oamUpdate(&oamMain);
  }

  return 0;
}
