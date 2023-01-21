#ifndef __H_MODEL__
#define __H_MODEL__

#include <stdint.h>
#include <time.h>
#include "mdx.h"
#include "voice.h"
#include "opm.h"

#define MAX_PATH_LEN (256)
#define MAX_MES_LEN  (128)

#define LIST_VIEW_SIZE    (7)
#define MESSAGE_VIEW_SIZE (5)

#define ADSR_WIDTH   (114)
#define ADSR_KEY_OFF (92)

#define SORT_ORDER_NORMAL  (0)
#define SORT_ORDER_REVERSE (1)

#define DEFAULT_FADEOUT_TIME  (180)

// MVC model
typedef struct {

  // high memory use
  int32_t use_high_memory;

  // for program and operators
  VOICE_SET* voice_set;
  int32_t voice_index;

  // for MDX list panel
  MDX_LIST* mdx_list;
  int32_t list_index;
  int32_t list_view_size;
  int32_t list_view_index;
  int32_t list_sort_order;

  // current playing mdx
  int32_t current_mdx_index;
  time_t current_mdx_start_time;

  // auto play
  int32_t auto_play_mode;
  int32_t auto_fadeout_time;

  // for message panel
  int32_t message_index;
  int32_t message_view_size;

  // ADSR chart data
  float adsr_m1[ ADSR_WIDTH ];
  float adsr_c1[ ADSR_WIDTH ];
  float adsr_m2[ ADSR_WIDTH ];
  float adsr_c2[ ADSR_WIDTH ];

  // wave chart data
  float wave_m1[ ADSR_WIDTH ];
  float wave_c1[ ADSR_WIDTH ];
  float wave_m2[ ADSR_WIDTH ];
  float wave_c2[ ADSR_WIDTH ];
  float wave_total[ ADSR_WIDTH ];

  // opm wave handle
  OPM_WAVE opm_wave;

} MODEL;

// prototype declarations
void model_init(MODEL* m);

#endif