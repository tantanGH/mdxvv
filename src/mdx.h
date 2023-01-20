#ifndef __H_MDX__
#define __H_MDX__

#include <stdio.h>
#include <stdint.h>
#include "voice.h"

#define MDX_VOICE_LEN (27)

#define MDX_MAX_PATH_NAME_LEN   (256)
#define MDX_MAX_FILE_NAME_LEN   (32)
#define MDX_MAX_DATA_TITLE_LEN  (256)

// MDX
typedef struct {

  uint8_t file_name[32];

  uint8_t* data_buffer;
  int32_t data_len;

  uint8_t* data_title;
  uint8_t* pcm_file_name;

  uint16_t voice_offset;   
  int32_t voice_count;

//  unsigned short channel_offset[16];    // no use in MDXVV

} MDX;

// MDX list (including sub directories)
typedef struct {

  uint8_t path_name [ 256 ];

  uint8_t* file_names;
  uint8_t* data_titles;
  int32_t* mdx_sort_indexes;
  int32_t mdx_count;

  uint8_t* sub_dir_names;
  int32_t* sub_dir_sort_indexes;
  int32_t sub_dir_count;

} MDX_LIST;

// prototype declarations for MDX
int32_t mdx_open(MDX* mdx, const uint8_t* file_name, int32_t use_high_memory);
void mdx_close(MDX* mdx, int32_t use_high_memory);
void mdx_describe(MDX* mdx);
VOICE_SET* mdx_get_voice_set(MDX* mdx, int32_t use_high_memory);

// prototype declarations for MDX_LIST
MDX_LIST* mdx_list_open(const uint8_t* dir_name, int32_t use_high_memory);
void mdx_list_close(MDX_LIST* mdx_list, int32_t use_high_memory);
uint8_t* mdx_list_get_sorted_file_name(MDX_LIST* mdx_list, int32_t mi, int32_t order);
uint8_t* mdx_list_get_sorted_data_title(MDX_LIST* mdx_list, int32_t mi, int32_t order);
uint8_t* mdx_list_get_sorted_sub_dir_name(MDX_LIST* mdx_list, int32_t si, int32_t order);

#endif