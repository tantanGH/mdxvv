#include <math.h>
#include "opm.h"

#define X_SCALE (1.5)

//
//  OPM ADSR envelope (not accurate!)
//
void opm_adsr(float* output, int num_samples, int key_off, int attack_rate, int decay_rate1, int decay_rate2, int release_rate, int decay_level1) {

  int attack_time  = X_SCALE * (OPM_MAX_ATTACK_RATE  - attack_rate );
  int decay_time1  = X_SCALE * (OPM_MAX_DECAY_RATE1  - decay_rate1 );
  int decay_time2  = X_SCALE * (OPM_MAX_DECAY_RATE2  - decay_rate2 );
  int release_time = X_SCALE * (OPM_MAX_RELEASE_RATE - release_rate);

  float decay_level  = (float)decay_level1 / (float)OPM_MAX_DECAY_LEVEL1;

  int n = 0;
    
  if (attack_time > 0) {
    while (n < attack_time) {
      output[n] = 1.0 - exp(-5.0 * n / attack_time);
      n++;
    }
  }
  
  if (decay_time1 > 0) {
    while (n < attack_time + decay_time1) {
      output[n] = decay_level + (1.0 - decay_level) * exp(-5.0 * (n - attack_time) / decay_time1);
      n++;
    }
  }
  
  if (decay_time2 > 0) {
    while (n < key_off) {
      output[n] = output[ attack_time + decay_time1 - 1 ] * exp(-5.0 * (n - attack_time - decay_time1) / decay_time2);
      n++;
    }
  }

  if (release_time > 0) {
    while (n < num_samples) {
      output[n] = output[ key_off - 1 ] * exp(-5.0 * (n - key_off + 1) / release_time);
      n++;
    }
  }

}

