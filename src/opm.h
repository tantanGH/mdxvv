#ifndef __H_OPM__
#define __H_OPM__

#include <stdint.h>

#define OPM_MAX_ATTACK_RATE   (31)
#define OPM_MAX_DECAY_RATE1   (31)
#define OPM_MAX_DECAY_RATE2   (31)
#define OPM_MAX_RELEASE_RATE  (15)
#define OPM_MAX_DECAY_LEVEL1  (15)

void opm_adsr(float* output, int32_t num_samples, int32_t key_off, int32_t attack_rate, int32_t decay_rate1, int32_t decay_rate2, int32_t release_rate, int32_t decay_level1);

#endif