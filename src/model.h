#ifndef __H_MODEL__
#define __H_MODEL__

#include <stdint.h>
#include "mdx.h"
#include "voice.h"

#define MAX_PATH_LEN (256)
#define MAX_MES_LEN  (128)

#define ADSR_WIDTH   (114)
#define ADSR_KEY_OFF (92)

#define SORT_ORDER_NORMAL  (0)
#define SORT_ORDER_REVERSE (1)

// MVC model
typedef struct {

  // for program and operators
  VOICE_SET* voice_set;
  int32_t voice_index;

  // ADSR chart data
  float adsr_m1[ ADSR_WIDTH ];
  float adsr_c1[ ADSR_WIDTH ];
  float adsr_m2[ ADSR_WIDTH ];
  float adsr_c2[ ADSR_WIDTH ];

  // for MDX list panel
  MDX_LIST* mdx_list;
  int32_t list_index;
  int32_t list_view_size;
  int32_t list_view_index;
  int32_t list_sort_order;

  // for message panel
  int32_t message_index;
  int32_t message_view_size;

  // current drive and directory preservation
  int32_t original_current_drive;
  uint8_t original_current_dir[ MAX_PATH_LEN ];

} MODEL;

#endif