#include <stdio.h>
#include <stdint.h>
#include "opm.h"

void main() {

  #define ENV_LEN (112)

  static float adsr_m1[ENV_LEN];
  static float adsr_c1[ENV_LEN];
  static float adsr_m2[ENV_LEN];  
  static float adsr_c2[ENV_LEN];
  
  static float wave_m1[ENV_LEN];
  static float wave_c1[ENV_LEN];
  static float wave_m2[ENV_LEN];
  static float wave_c2[ENV_LEN];
  static float wave_total[ENV_LEN];
   
  VOICE v = { 0 };
  v.attack_rate_m1 = 23;
  v.decay_rate1_m1 = 12;
  v.decay_rate2_m1 = 4;
  v.release_rate_m1 = 7;
  v.decay_level1_m1 = 4;
  v.total_level_m1 = 30;
  v.key_scaling_m1 = 1;
  v.phase_multi_m1 = 0;
  v.detune1_m1 = 0;
  v.detune2_m1 = 0;
  v.ams_enable_m1 = 0;

  v.attack_rate_c1 = 27;
  v.decay_rate1_c1 = 13;
  v.decay_rate2_c1 = 4;
  v.release_rate_c1 = 7;
  v.decay_level1_c1 = 4;
  v.total_level_c1 = 32;
  v.key_scaling_c1 = 3;
  v.phase_multi_c1 = 5;
  v.detune1_c1 = 0;
  v.detune2_c1 = 0;
  v.ams_enable_c1 = 0;

  v.attack_rate_m2 = 28;
  v.decay_rate1_m2 = 12;
  v.decay_rate2_m2 = 4;
  v.release_rate_m2 = 7;
  v.decay_level1_m2 = 4;
  v.total_level_m2 = 32;
  v.key_scaling_m2 = 1;
  v.phase_multi_m2 = 0;
  v.detune1_m2 = 0;
  v.detune2_m2 = 0;
  v.ams_enable_m2 = 0;

  v.attack_rate_c2 = 26;
  v.decay_rate1_c2 = 11;
  v.decay_rate2_c2 = 4;
  v.release_rate_c2 = 9;
  v.decay_level1_c2 = 4;
  v.total_level_c2 = 0;
  v.key_scaling_c2 = 1;
  v.phase_multi_c2 = 1;
  v.detune1_c2 = 0;
  v.detune2_c2 = 0;
  v.ams_enable_c2 = 0;

  OPM_WAVE ow = { 0 };
  ow.num_samples = ENV_LEN;
  ow.available = 0;
  ow.voice = &v;
  ow.adsr_m1 = adsr_m1;
  ow.adsr_c1 = adsr_c1;
  ow.adsr_m2 = adsr_m2;
  ow.adsr_c2 = adsr_c2;
  ow.wave_m1 = wave_m1;
  ow.wave_c1 = wave_c1;
  ow.wave_m2 = wave_m2;
  ow.wave_c2 = wave_c2;
  ow.wave_total = wave_total;

  opm_adsr(adsr_m1, ENV_LEN, 80, 25, 12, 5, 6, 4);
  opm_adsr(adsr_c1, ENV_LEN, 80, 20, 11, 4, 5, 2);
  opm_adsr(adsr_m2, ENV_LEN, 80, 31,  8, 3, 8, 4);
  opm_adsr(adsr_c2, ENV_LEN, 80, 24,  9, 1, 3, 5);

  opm_wave_plot(&ow, 30);

  for (int32_t i = 0; i < ENV_LEN; i++) {
    printf("wave_m1[%d]=%f\n",i,wave_m1[i]);
//    printf("adsr_c1[%d]=%f\n",i,adsr_c1[i]);
  }
  for (int32_t i = 0; i < ENV_LEN; i++) {
    printf("wave_c1[%d]=%f\n",i,wave_c1[i]);
//    printf("adsr_c1[%d]=%f\n",i,adsr_c1[i]);
  }
  for (int32_t i = 0; i < ENV_LEN; i++) {
    printf("wave_m2[%d]=%f\n",i,wave_m2[i]);
//    printf("adsr_c1[%d]=%f\n",i,adsr_c1[i]);
  }
  for (int32_t i = 0; i < ENV_LEN; i++) {
    printf("wave_c2[%d]=%f\n",i,wave_c2[i]);
//    printf("adsr_c1[%d]=%f\n",i,adsr_c1[i]);
  }

}