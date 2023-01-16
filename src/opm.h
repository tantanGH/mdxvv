#ifndef __H_OPM__
#define __H_OPM__

#define OPM_MAX_ATTACK_RATE   (31)
#define OPM_MAX_DECAY_RATE1   (31)
#define OPM_MAX_DECAY_RATE2   (31)
#define OPM_MAX_RELEASE_RATE  (15)
#define OPM_MAX_DECAY_LEVEL1  (15)

void opm_adsr(float* output, int num_samples, int key_off, int attack_rate, int decay_rate1, int decay_rate2, int release_rate, int decay_level1);

#endif