#ifndef __H_VOICE__
#define __H_VOICE__

#include <stdio.h>
#include <stddef.h>
#include <class.h>

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

  unsigned int voice_id;

//  time_t create_time;
//  time_t update_time;
  
  unsigned char version[8];

  unsigned char name[24];
//  unsigned char tag1[8];
//  unsigned char tag2[8];
//  unsigned char tag3[8];
//  unsigned char tag4[8];
  unsigned char comment[64];

//  unsigned char favorite;
//  unsigned char selected;         // for UI use only (not serialized)
//  unsigned char deleted;          // for UI use only (not serialized)

  unsigned char connection;
  unsigned char feedback;
  unsigned char slot_mask;
  unsigned char wave_form;
  unsigned char synchro;
  unsigned char speed;
  unsigned char pmd;
  unsigned char amd;
  unsigned char pms;
  unsigned char ams;
  unsigned char pan;
  unsigned char reserved;

  unsigned char attack_rate_m1;
  unsigned char decay_rate1_m1;
  unsigned char decay_rate2_m1;
  unsigned char release_rate_m1;
  unsigned char decay_level1_m1;
  unsigned char total_level_m1;
  unsigned char key_scaling_m1;
  unsigned char phase_multi_m1;
  unsigned char detune1_m1;
  unsigned char detune2_m1;
  unsigned char ams_enable_m1;

  unsigned char attack_rate_c1;
  unsigned char decay_rate1_c1;
  unsigned char decay_rate2_c1;
  unsigned char release_rate_c1;
  unsigned char decay_level1_c1;
  unsigned char total_level_c1;
  unsigned char key_scaling_c1;
  unsigned char phase_multi_c1;
  unsigned char detune1_c1;
  unsigned char detune2_c1;
  unsigned char ams_enable_c1;  

  unsigned char attack_rate_m2;
  unsigned char decay_rate1_m2;
  unsigned char decay_rate2_m2;
  unsigned char release_rate_m2;
  unsigned char decay_level1_m2;
  unsigned char total_level_m2;
  unsigned char key_scaling_m2;
  unsigned char phase_multi_m2;
  unsigned char detune1_m2;
  unsigned char detune2_m2;
  unsigned char ams_enable_m2;

  unsigned char attack_rate_c2;
  unsigned char decay_rate1_c2;
  unsigned char decay_rate2_c2;
  unsigned char release_rate_c2;
  unsigned char decay_level1_c2;
  unsigned char total_level_c2;
  unsigned char key_scaling_c2;
  unsigned char phase_multi_c2;
  unsigned char detune1_c2;
  unsigned char detune2_c2;
  unsigned char ams_enable_c2;

} VOICE;

typedef struct {

//  unsigned int voice_set_id;
//  unsigned char version[8];

//  time_t create_time;
//  time_t update_time;

  unsigned char name[32];
//  unsigned char tag1[8];
//  unsigned char tag2[8];
//  unsigned char tag3[8];
//  unsigned char tag4[8];
  unsigned char comment[256];

  int voice_count;
  VOICE* voices;

} VOICE_SET;

// prototype declarations for VOICE_SET
int voice_set_fwrite(VOICE_SET* vs, FILE* fp, int format);
void voice_set_close(VOICE_SET* vs);

#endif