#include <stdint.h>
#include <math.h>
#include "opm.h"

//
//  OPM ADSR envelope (not accurate!)
//
void opm_adsr(float* output, int32_t num_samples, int32_t key_off, int32_t attack_rate, int32_t decay_rate1, int32_t decay_rate2, int32_t release_rate, int32_t decay_level1) {

  int32_t attack_time  = OPM_X_SCALE * (OPM_MAX_ATTACK_RATE  - attack_rate );
  int32_t decay_time1  = OPM_X_SCALE * (OPM_MAX_DECAY_RATE1  - decay_rate1 );
  int32_t decay_time2  = OPM_X_SCALE * (OPM_MAX_DECAY_RATE2  - decay_rate2 );
  int32_t release_time = OPM_X_SCALE * (OPM_MAX_RELEASE_RATE - release_rate);

  float decay_level  = (float)decay_level1 / (float)OPM_MAX_DECAY_LEVEL1;

  int32_t n = 0;
    
  if (attack_time > 0) {
    while (n < attack_time) {
      output[n] = 1.0 - exp(-5.0 * n / (float)attack_time);
      n++;
    }
  }
  
  if (decay_time1 > 0) {
    while (n < attack_time + decay_time1) {
      output[n] = decay_level + (1.0 - decay_level) * exp(-5.0 * (n - attack_time) / (float)decay_time1);
      n++;
    }
  }
  
  if (decay_time2 > 0) {
    while (n < key_off && n < num_samples) {
      output[n] = output[ attack_time + decay_time1 - 1 ] * exp(-5.0 * (n - attack_time - decay_time1) / (float)decay_time2);
      n++;
    }
  }

  if (release_time > 0) {
    while (n < num_samples) {
      output[n] = output[ key_off - 1 ] * exp(-5.0 * (n - key_off + 1) / (float)release_time);
      n++;
    }
  }

}

//
//  OPM wave output (not accurate!!)
//
void opm_wave_plot(OPM_WAVE* ow, size_t len) {

  int n = ow->available;

  if (n == 0) {

    VOICE* v = ow->voice;

    ow->freq_c1 = OPM_DEFAULT_FREQ;
    ow->freq_c2 = OPM_DEFAULT_FREQ;

    ow->freq_c1 *= pow(2.0, v->detune1_c1 / 24.0);
    ow->freq_c2 *= pow(2.0, v->detune1_c2 / 24.0);
    
    ow->freq_c1 *= pow(2.0, v->key_scaling_c1 / 36.0);
    ow->freq_c2 *= pow(2.0, v->key_scaling_c2 / 36.0);

    ow->freq_m1 = ow->freq_c1 * (v->phase_multi_m1 + 1);
    ow->freq_m2 = ow->freq_c2 * (v->phase_multi_m2 + 1);

    ow->freq_m1 *= pow(2.0, v->detune2_m1 / 24.0);
    ow->freq_m2 *= pow(2.0, v->detune2_m2 / 24.0);

  }

  for (int32_t i = 0; i < len && n < ow->num_samples; i++) {

    ow->wave_m1[n] = ow->adsr_m1[n] * sin( 2.0 * PI * ow->freq_m1 * n * OPM_X_SCALE );
    ow->wave_m2[n] = ow->adsr_m2[n] * sin( 2.0 * PI * ow->freq_m2 * n * OPM_X_SCALE );

    ow->wave_c1[n] = ow->adsr_c1[n] * sin( 2.0 * PI * ow->freq_c1 * n * OPM_X_SCALE + ow->wave_m1[n] );
    ow->wave_c2[n] = ow->adsr_c2[n] * sin( 2.0 * PI * ow->freq_c2 * n * OPM_X_SCALE + ow->wave_m2[n] );

    ow->wave_total[n] = ow->wave_c1[n] + ow->wave_c2[n];

    n++;
  }

  ow->available = n;
}
