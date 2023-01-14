#ifndef __H_MODEL__
#define __H_MODEL__

#include "mdx.h"
#include "voice.h"

#define MAX_PATH_LEN (256)
#define MAX_MES_LEN  (128)

// MVC model
typedef struct {

  // for program and operators
  VOICE_SET* voice_set;
  int voice_index;

  // for MDX list panel
  MDX_LIST* mdx_list;
  int list_index;
  int list_view_size;
  int list_view_index;

  // for message panel
  int message_index;
  int message_view_size;

  // current drive and directory preservation
  int original_current_drive;
  char original_current_dir[ MAX_PATH_LEN ];

  // pause/resume operations
  int stopped_mdx;

} MODEL;

#endif