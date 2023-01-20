#ifndef __H_OPM__
#define __H_OPM__

#include <stdint.h>
#include "voice.h"

#define OPM_MAX_ATTACK_RATE   (31)
#define OPM_MAX_DECAY_RATE1   (31)
#define OPM_MAX_DECAY_RATE2   (31)
#define OPM_MAX_RELEASE_RATE  (15)
#define OPM_MAX_DECAY_LEVEL1  (15)

#define OPM_X_SCALE (1.8)

#define OPM_DEFAULT_FREQ (440)

typedef struct {

  int32_t num_samples;
  int32_t available;

  VOICE* voice;

  float freq_m1;
  float freq_c1;
  float freq_m2;
  float freq_c2;

  float* adsr_m1;
  float* adsr_c1;
  float* adsr_m2;
  float* adsr_c2;

  float* wave_m1;
  float* wave_c1;
  float* wave_m2;
  float* wave_c2;

  float* wave_total;

} OPM_WAVE;

void opm_adsr(float* output, int32_t num_samples, int32_t key_off, int32_t attack_rate, int32_t decay_rate1, int32_t decay_rate2, int32_t release_rate, int32_t decay_level1);
void opm_wave_plot(OPM_WAVE* ow, size_t len);

#endif