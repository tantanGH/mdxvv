#ifndef __H_MDX__
#define __H_MDX__

#include <stdio.h>
#include "voice.h"

#define MDX_VOICE_LEN (27)

#define MDX_MAX_PATH_NAME_LEN   (256)
#define MDX_MAX_FILE_NAME_LEN   (32)
#define MDX_MAX_DATA_TITLE_LEN  (256)

// MDX
typedef struct {

  unsigned char file_name[32];

  unsigned char* data_buffer;
  int data_len;

  unsigned char* data_title;
  unsigned char* pcm_file_name;

  unsigned short voice_offset;   
  int voice_count;

  unsigned short channel_offset[16];

} MDX;

// MDX list (including sub directories)
typedef struct {

  unsigned char path_name [ 256 ];

  unsigned char* file_names;
  unsigned char* data_titles;
  int* mdx_sort_indexes;
  int mdx_count;

  unsigned char* sub_dir_names;
  int* sub_dir_sort_indexes;
  int sub_dir_count;

} MDX_LIST;

// prototype declarations for MDX
int mdx_open(MDX* mdx, const unsigned char* file_name);
void mdx_close(MDX* mdx);
void mdx_describe(MDX* mdx);
VOICE_SET* mdx_get_voice_set(MDX* mdx);

// prototype declarations for MDX_LIST
MDX_LIST* mdx_list_open(const unsigned char* dir_name);
unsigned char* mdx_list_get_sorted_file_name(MDX_LIST* mdx_list, int mi);
unsigned char* mdx_list_get_sorted_data_title(MDX_LIST* mdx_list, int mi);
unsigned char* mdx_list_get_sorted_sub_dir_name(MDX_LIST* mdx_list, int si);
void mdx_list_close(MDX_LIST* mdx_list);

#endif