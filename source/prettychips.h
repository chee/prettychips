#include "adsr.h"
#include <nds.h>

typedef struct {
  int sound_id;
  ADSR *envelope;
  u8 distance_from_root;
  KEYPAD_BITS button;
} Button;

extern enum Scale scale;
enum Scale {
  scale_major,
  scale_minor,
  scale_melodic,
  scale_phrygian,
  scale_blues
};

typedef struct {
  float attack_rate;
  float decay_rate;
  float release_rate;
  float sustain_level;
  float octave;
  float key;
  enum Scale scale;
  DutyCycle wave_duty;
} PrettyChipsState;
