#ifndef __H_VOICE__
#define __H_VOICE__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define FORMAT_MDX  (0)
#define FORMAT_ZMS  (1)
#define FORMAT_XC   (2)
#define FORMAT_BAS  (3)

#define MAX_CONNECTION   (7)
#define MAX_FEEDBACK     (7)
#define MAX_SLOT_MASK    (15)

#define MAX_ATTACK_RATE  (31)
#define MAX_DECAY_RATE1  (31)
#define MAX_DECAY_RATE2  (31)
#define MAX_RELEASE_RATE (15)
#define MAX_DECAY_LEVEL1 (15)
#define MAX_TOTAL_LEVEL  (127)
#define MAX_KEY_SCALING  (3)
#define MAX_PHASE_MULTI  (15)
#define MAX_DETUNE1      (7)
#define MAX_DETUNE2      (3)
#define MAX_AMS_ENABLE   (1)

typedef struct {

  int32_t voice_id;

  uint8_t version[8];
  uint8_t name[24];
  uint8_t comment[64];

  uint8_t connection;
  uint8_t feedback;
  uint8_t slot_mask;
  uint8_t wave_form;
  uint8_t synchro;
  uint8_t speed;
  uint8_t pmd;
  uint8_t amd;
  uint8_t pms;
  uint8_t ams;
  uint8_t pan;
  uint8_t reserved;

  uint8_t attack_rate_m1;
  uint8_t decay_rate1_m1;
  uint8_t decay_rate2_m1;
  uint8_t release_rate_m1;
  uint8_t decay_level1_m1;
  uint8_t total_level_m1;
  uint8_t key_scaling_m1;
  uint8_t phase_multi_m1;
  uint8_t detune1_m1;
  uint8_t detune2_m1;
  uint8_t ams_enable_m1;

  uint8_t attack_rate_c1;
  uint8_t decay_rate1_c1;
  uint8_t decay_rate2_c1;
  uint8_t release_rate_c1;
  uint8_t decay_level1_c1;
  uint8_t total_level_c1;
  uint8_t key_scaling_c1;
  uint8_t phase_multi_c1;
  uint8_t detune1_c1;
  uint8_t detune2_c1;
  uint8_t ams_enable_c1;  

  uint8_t attack_rate_m2;
  uint8_t decay_rate1_m2;
  uint8_t decay_rate2_m2;
  uint8_t release_rate_m2;
  uint8_t decay_level1_m2;
  uint8_t total_level_m2;
  uint8_t key_scaling_m2;
  uint8_t phase_multi_m2;
  uint8_t detune1_m2;
  uint8_t detune2_m2;
  uint8_t ams_enable_m2;

  uint8_t attack_rate_c2;
  uint8_t decay_rate1_c2;
  uint8_t decay_rate2_c2;
  uint8_t release_rate_c2;
  uint8_t decay_level1_c2;
  uint8_t total_level_c2;
  uint8_t key_scaling_c2;
  uint8_t phase_multi_c2;
  uint8_t detune1_c2;
  uint8_t detune2_c2;
  uint8_t ams_enable_c2;

} VOICE;

typedef struct {

  uint8_t name[32];
  uint8_t comment[256];

  int32_t voice_count;
  VOICE* voices;

} VOICE_SET;

// prototype declarations for VOICE_SET
int32_t voice_set_fwrite(VOICE_SET* vs, FILE* fp, int32_t format);
void voice_set_close(VOICE_SET* vs, int32_t use_high_memory);

#endif