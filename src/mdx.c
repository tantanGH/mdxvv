#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <doslib.h>
#include "mdx.h"
#include "voice.h"
#include "memory.h"

#define HIGH_MEMORY_USE  (0)

// open MDX
int mdx_open(MDX* mdx, const unsigned char* file_name) {

  int rc = 0;

  // read MDX file content to memory buffer
  FILE* fp = NULL;
  if ((fp = fopen(file_name, "rb")) != NULL) {

    // get file size
    fseek(fp, 0, SEEK_END);
    mdx->data_len = (int)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // allocate memory buffer and read
    mdx->data_buffer = malloc_himem(mdx->data_len, HIGH_MEMORY_USE);
    fread(mdx->data_buffer, 1, mdx->data_len, fp);
    
    // close
    fclose(fp);
    fp = NULL;    

    // file name
    struct NAMESTBUF nb = { 0 };

    static char name1[] = "        ";
    static char name2[] = "          ";
    static char ext[] = "   ";
     
    NAMESTS((unsigned char*)file_name, &nb);
    memcpy(name1, nb.name1, 8);
    memcpy(name2, nb.name2, 10);
    memcpy(ext, nb.ext, 3);

    for (int i = 0; i < 8; i++) {
      if (name1[i] == ' ') {
        name1[i] = '\0';
        break;
      } else if (name1[i] == '\0') {
        break;
      }
    }

    for (int i = 0; i < 10; i++) {
      if (name2[i] == ' ') {
        name2[i] = '\0';
        break;
      } else if (name2[i] == '\0') {
        break;
      }
    }

    for (int i = 0; i < 3; i++) {
      if (ext[i] == ' ') {
        ext[i] = '\0';
        break;
      } else if (ext[i] == '\0') {
        break;
      }
    }

    sprintf(mdx->file_name,"%s%s.%s", name1, name2, ext);

  } else {
    rc = -1;
    goto catch;
  }

  // parse data title
  mdx->data_title = NULL;
  int ofs = 0;
  while ( ofs + 2 < mdx->data_len ) {
    if (mdx->data_buffer[ ofs ] == 0x0d && mdx->data_buffer[ ofs + 1 ] == 0x0a && mdx->data_buffer[ ofs + 2 ] == 0x1a ) {
          mdx->data_title = mdx->data_buffer;
          mdx->data_title[ ofs ] = '\0';
          break;
    }
    ofs++;
  }
  ofs += 3;
  if (ofs >= mdx->data_len) {
    rc = -2;
    goto catch;
  }

  // parse pcm file name
  mdx->pcm_file_name = NULL;
  int ofs_pcm = ofs;
  while ( ofs < mdx->data_len ) {
    if (mdx->data_buffer[ ofs++ ] == 0x00) {
      mdx->pcm_file_name = mdx->data_buffer + ofs_pcm;
      break;
    }
  }
  if (ofs >= mdx->data_len) {
    rc = -2;
    goto catch;
  }

  // channel and tone data offset base
  int ofs_base = ofs;

  // voice data offset
  mdx->voice_offset = ofs_base + ( mdx->data_buffer[ ofs ] * 256 ) + mdx->data_buffer[ ofs + 1 ];
  if (mdx->voice_offset > mdx->data_len) {
    rc = -2;
    goto catch;
  }
  ofs += 2;

  // count voices
  mdx->voice_count = ( mdx->data_len - mdx->voice_offset ) / MDX_VOICE_LEN;

  // channel data offset (no use in MDXVV)
/*
  for (int i = 0; i < 9; i++) {   // OPM 8 + PCM 1
    mdx->channel_offset[i] = ofs_base + ( mdx->data_buffer[ ofs ] * 256 ) + mdx->data_buffer[ ofs + 1 ];
    if (mdx->channel_offset[i] > mdx->data_len) {
      rc = -2;
      goto catch;
    }
    ofs += 2;
  }
  if (mdx->channel_offset[0] >= ofs_base + 0x22) {   // ex-pcm mode?
    for (int i = 9; i < 16; i++) {
      mdx->channel_offset[i] = ofs_base + ( mdx->data_buffer[ ofs ] * 256 ) + mdx->data_buffer[ ofs + 1 ];
      if (mdx->channel_offset[i] > mdx->data_len) {
        rc = -2;
        goto catch;
      }
      ofs += 2;
    }
  } else {
    for (int i = 9; i < 16; i++) {
      mdx->channel_offset[i] = 0;
    }
  }
*/
  goto exit;

catch:
  mdx->data_len = 0;
  if (mdx->data_buffer != NULL) {
    free_himem(mdx->data_buffer, HIGH_MEMORY_USE);
    mdx->data_buffer = NULL;
  }

exit:
  return rc;
}

// close MDX
void mdx_close(MDX* mdx) {
  if (mdx->data_buffer != NULL) {
    free_himem(mdx->data_buffer, HIGH_MEMORY_USE);
    mdx->data_buffer = NULL;
  }
}

// get voice set
VOICE_SET* mdx_get_voice_set(MDX* mdx) {

  if (mdx->voice_count <= 0 || mdx->data_buffer == NULL) return NULL;

  VOICE_SET* vs = malloc_himem(sizeof(VOICE_SET), HIGH_MEMORY_USE);

//  vs->voice_set_id = time(NULL);
//  strcpy(vs->version, "TBD");

//  vs->create_time = time(NULL);
//  vs->update_time = vs->create_time;
  
  strcpy(vs->name, mdx->file_name);

//  vs->tag1[0] = '\0';
//  vs->tag2[0] = '\0';
//  vs->tag3[0] = '\0';
//  vs->tag4[0] = '\0';

  strcpy(vs->comment, mdx->data_title);

  vs->voice_count = mdx->voice_count;
  vs->voices = malloc_himem(sizeof(VOICE) * vs->voice_count, HIGH_MEMORY_USE);

  for (int i = 0; i < vs->voice_count; i++) {

    VOICE* v = &(vs->voices[i]);

    v->voice_id = i;
//    v->create_time = vs->create_time;
//    v->update_time = vs->update_time;
    v->name[0] = '\0';
//    v->tag1[0] = '\0';
//    v->tag2[0] = '\0';
//    v->tag3[0] = '\0';
//    v->tag4[0] = '\0';
    v->comment[0] = '\0';
//    v->favorite = 0;
//    v->selected = 0;
//    v->deleted = 0;

    unsigned char* vbuf = &(mdx->data_buffer[ mdx->voice_offset + i * MDX_VOICE_LEN ]);

    v->voice_id = vbuf[0];

    v->connection = ( vbuf[1] ) & 0x07;
    v->feedback = ( vbuf[1] >> 3 ) & 0x07;
    v->slot_mask = ( vbuf[2] ) & 0x0f;
    v->wave_form = 0;
    v->synchro = 0;
    v->speed = 0;
    v->pmd = 0;
    v->amd = 0;
    v->pms = 0;
    v->ams = 0;
    v->pan = 3;
    v->reserved = 0;

    v->detune1_m1 = ( vbuf[3] >> 4 ) & 0x07;
    v->detune1_m2 = ( vbuf[4] >> 4 ) & 0x07;
    v->detune1_c1 = ( vbuf[5] >> 4 ) & 0x07;
    v->detune1_c2 = ( vbuf[6] >> 4 ) & 0x07;

    v->phase_multi_m1 = ( vbuf[3] ) & 0x0f;
    v->phase_multi_m2 = ( vbuf[4] ) & 0x0f;
    v->phase_multi_c1 = ( vbuf[5] ) & 0x0f;
    v->phase_multi_c2 = ( vbuf[6] ) & 0x0f;

    v->total_level_m1 = ( vbuf[7] ) & 0x7f;
    v->total_level_m2 = ( vbuf[8] ) & 0x7f;
    v->total_level_c1 = ( vbuf[9] ) & 0x7f;
    v->total_level_c2 = ( vbuf[10] ) & 0x7f;

    v->key_scaling_m1 = ( vbuf[11] >> 6 ) & 0x03;
    v->key_scaling_m2 = ( vbuf[12] >> 6 ) & 0x03;
    v->key_scaling_c1 = ( vbuf[13] >> 6 ) & 0x03;
    v->key_scaling_c2 = ( vbuf[14] >> 6 ) & 0x03;

    v->attack_rate_m1 = ( vbuf[11] ) & 0x1f;
    v->attack_rate_m2 = ( vbuf[12] ) & 0x1f;
    v->attack_rate_c1 = ( vbuf[13] ) & 0x1f;
    v->attack_rate_c2 = ( vbuf[14] ) & 0x1f;

    v->ams_enable_m1 = ( vbuf[15] >> 7 ) & 0x01;
    v->ams_enable_m2 = ( vbuf[16] >> 7 ) & 0x01;
    v->ams_enable_c1 = ( vbuf[17] >> 7 ) & 0x01;
    v->ams_enable_c2 = ( vbuf[18] >> 7 ) & 0x01;

    v->decay_rate1_m1 = ( vbuf[15] ) & 0x1f;
    v->decay_rate1_m2 = ( vbuf[16] ) & 0x1f;
    v->decay_rate1_c1 = ( vbuf[17] ) & 0x1f;
    v->decay_rate1_c2 = ( vbuf[18] ) & 0x1f;

    v->detune2_m1 = ( vbuf[19] >> 6 ) & 0x03;
    v->detune2_m2 = ( vbuf[20] >> 6 ) & 0x03;
    v->detune2_c1 = ( vbuf[21] >> 6 ) & 0x03;
    v->detune2_c2 = ( vbuf[22] >> 6 ) & 0x03;

    v->decay_rate2_m1 = ( vbuf[19] ) & 0x1f;
    v->decay_rate2_m2 = ( vbuf[20] ) & 0x1f;
    v->decay_rate2_c1 = ( vbuf[21] ) & 0x1f;
    v->decay_rate2_c2 = ( vbuf[22] ) & 0x1f;

    v->decay_level1_m1 = ( vbuf[23] >> 4 ) & 0x0f;
    v->decay_level1_m2 = ( vbuf[24] >> 4 ) & 0x0f;
    v->decay_level1_c1 = ( vbuf[25] >> 4 ) & 0x0f;
    v->decay_level1_c2 = ( vbuf[26] >> 4 ) & 0x0f;

    v->release_rate_m1 = ( vbuf[23] ) & 0x0f;
    v->release_rate_m2 = ( vbuf[24] ) & 0x0f;
    v->release_rate_c1 = ( vbuf[25] ) & 0x0f;
    v->release_rate_c2 = ( vbuf[26] ) & 0x0f;
  }

  return vs;
}

// quick sort helper
static unsigned char* g_mdx_file_names;   // must be set beforehand
static int compare_mdx_file_names(const void* n1, const void* n2) {
  int idx1 = *((int*)n1);
  int idx2 = *((int*)n2);
  unsigned char* str1 = g_mdx_file_names + MDX_MAX_FILE_NAME_LEN * idx1;
  unsigned char* str2 = g_mdx_file_names + MDX_MAX_FILE_NAME_LEN * idx2;
  return stricmp(str1,str2);
}

// quick sort helper
static unsigned char* g_sub_dir_names;    // must be set beforehand
static int compare_sub_dir_names(const void* n1, const void* n2) {
  int idx1 = *((int*)n1);
  int idx2 = *((int*)n2);
  unsigned char* str1 = g_sub_dir_names + MDX_MAX_FILE_NAME_LEN * idx1;
  unsigned char* str2 = g_sub_dir_names + MDX_MAX_FILE_NAME_LEN * idx2;
  return stricmp(str1,str2);
}

// describe MDX
void mdx_describe(MDX* mdx) {
  printf("file name: [%s]\n", mdx->file_name);
  printf("data title: [%s]\n", mdx->data_title);
  printf("pcm name: [%s]\n", mdx->pcm_file_name);
  printf("voice offset: %d\n", mdx->voice_offset);
  printf("voice count: %d\n", mdx->voice_count);
  //for (int i = 0; i < 8; i++) {
  //  printf("channel offset %C: %d\n", 'A'+i, mdx->channel_offset[i]);
  //}
  //for (int i = 0; i < 8; i++) {
  //  printf("channel offset %C: %d\n", 'P'+i, mdx->channel_offset[i+8]);
  //}
}

// open new MDX list
MDX_LIST* mdx_list_open(const unsigned char* dir_name) {

  MDX_LIST* mdx_list = NULL;
  static unsigned char dir_name2[ MDX_MAX_PATH_NAME_LEN ];

  // add "*.*" if it ends with ":" or path separator
  strcpy(dir_name2, dir_name);
  unsigned char uc = dir_name2[ strlen(dir_name2) - 1 ];
  if (uc == '\\' || uc == '/' || uc == ':') {
    strcat(dir_name2, "*.*");
  } else if (uc == '.') {
    if (strlen(dir_name2) >= 2) {
      unsigned char uc2 = dir_name2[ strlen(dir_name2) - 2 ];
      if (uc2 == '.') {
        // path separator + parent dir (..) -> add "\\*.*"
        strcat(dir_name2, "\\*.*");
      } else if (uc2 == '\\' || uc2 == '/' || uc2 == ':') {
        // path separator + current dir (.) -> replace "." with "*.*" 
        dir_name2[ strlen(dir_name2) - 1 ] = '\0';
        strcat(dir_name2, "*.*");
      }
    } else {
      // current dir (.) -> add "\\*.*"
      strcat(dir_name2, "\\*.*");
    }
  }

  // is this a file or a directory?
  static struct FILBUF filbuf;
  if (FILES(&filbuf, (unsigned char*)dir_name2, 0x33) < 0) {
    // not a normal file, not a directory
    goto exit;
  }

  // if this is a directory and not end with "*", add "\\*.*"
  uc = dir_name2[ strlen(dir_name2) - 1 ];
  if (filbuf.atr & 0x10 && uc != '*') {
    strcat(dir_name2, "\\*.*");
  }

  // get dir name
  static struct NAMESTBUF nb;
  if (NAMESTS((unsigned char*)dir_name2, &nb) < 0) {
    goto exit;
  }

  static unsigned char path_name[ MDX_MAX_PATH_NAME_LEN ];
  //static unsigned char selected_file_name[ MDX_MAX_FILE_NAME_LEN ];

  path_name[0] = 'A' + nb.drive;
  path_name[1] = ':';
  strcpy(path_name+2, nb.path);

  // 1st pass: count mdx files and sub dirs in the dir
  static unsigned char wild_name[ MDX_MAX_PATH_NAME_LEN ];
  strcpy(wild_name, path_name);
  strcat(wild_name, "*.*");
  int file_count = 0;
  int sub_dir_count = 0;
  if (FILES(&filbuf, wild_name, 0x33) >= 0) {
    do {
      if (filbuf.atr & 0x10) {
        sub_dir_count++;
      } else {
        file_count++;
      }
    } while (NFILES(&filbuf) >= 0);
  }

  // allocate memory
  mdx_list = (MDX_LIST*)malloc_himem(sizeof(MDX_LIST), HIGH_MEMORY_USE);
  mdx_list->file_names = file_count > 0 ? malloc_himem(MDX_MAX_FILE_NAME_LEN * file_count, HIGH_MEMORY_USE) : NULL;
  mdx_list->data_titles = file_count > 0 ? malloc_himem(MDX_MAX_DATA_TITLE_LEN * file_count, HIGH_MEMORY_USE) : NULL;
  mdx_list->mdx_count = 0;
  mdx_list->sub_dir_names = sub_dir_count > 0 ? malloc_himem(MDX_MAX_FILE_NAME_LEN * sub_dir_count, HIGH_MEMORY_USE) : NULL;
  mdx_list->sub_dir_count = 0;

  strcpy(mdx_list->path_name, path_name);
//  strcat(mdx_list->path_name, "\\");

  // 2nd pass: scan mdx file and get titles, as well as sub dirs
  int mi = 0;
  int si = 0;
  if (FILES(&filbuf, wild_name, 0x37) >= 0) {
    do {

      if ((filbuf.atr & 0x10) && !(filbuf.name[0] == '.' && filbuf.name[1] == '\0')) {

        // sub dir
        unsigned char* sub_dir_name = mdx_list->sub_dir_names + MDX_MAX_FILE_NAME_LEN * si;
        strcpy(sub_dir_name, filbuf.name);
        si++;

      } else if (stricmp(filbuf.name + strlen(filbuf.name) - 4, ".mdx") == 0) {

        // MDX file candidate
        unsigned char mdx_full_path_name[ MDX_MAX_PATH_NAME_LEN ];
        strcpy(mdx_full_path_name, path_name);
        strcat(mdx_full_path_name, "\\");
        strcat(mdx_full_path_name, filbuf.name);

        MDX mdx;
        if (mdx_open(&mdx, mdx_full_path_name) == 0) {
          unsigned char* file_name = mdx_list->file_names + MDX_MAX_FILE_NAME_LEN * mi;
          unsigned char* data_title = mdx_list->data_titles + MDX_MAX_DATA_TITLE_LEN * mi;
          strcpy(file_name, filbuf.name);
          strcpy(data_title, mdx.data_title);
          //if (strcmp(filbuf.name, selected_file_name)) {
          //  mdx_list->mdx_selected = i;
          //}         
          mdx_close(&mdx);
          mi++;
        }

      }

    } while (NFILES(&filbuf) >= 0);
  }

  mdx_list->mdx_count = mi;
  mdx_list->sub_dir_count = si;

  // sort mdx file list
  if (mdx_list->mdx_count > 0) {
    mdx_list->mdx_sort_indexes = malloc_himem(sizeof(int) * file_count, HIGH_MEMORY_USE);
    for (int i = 0; i < mdx_list->mdx_count; i++) {
      mdx_list->mdx_sort_indexes[i] = i;
    }
    g_mdx_file_names = mdx_list->file_names;
    qsort(mdx_list->mdx_sort_indexes, mdx_list->mdx_count, sizeof(int), &compare_mdx_file_names);
  } else {
    mdx_list->mdx_sort_indexes = NULL;
  }

  // sort sub dir list
  if (mdx_list->sub_dir_count > 0) {
    mdx_list->sub_dir_sort_indexes = malloc_himem(sizeof(int) * mdx_list->sub_dir_count, HIGH_MEMORY_USE);
    for (int i = 0; i < mdx_list->sub_dir_count; i++) {
      mdx_list->sub_dir_sort_indexes[i] = i;
    }
    g_sub_dir_names = mdx_list->sub_dir_names;
    qsort(mdx_list->sub_dir_sort_indexes, mdx_list->sub_dir_count, sizeof(int), &compare_sub_dir_names);
  } else {
    mdx_list->sub_dir_sort_indexes = NULL;
  }

exit:
  // NOTE: In case the target directory is root directory or a symbolic link, there is no '..' in the sub_dir list.
  // In that case, list object with mdx_count=0 && sub_dir_count=0 could be returned.
  return mdx_list;
}

// return sorted file name pointer
unsigned char* mdx_list_get_sorted_file_name(MDX_LIST* mdx_list, int mi, int order) {
  unsigned char* p;
  if (mdx_list != NULL) {
    int mi_sorted = (order == 0) ? mdx_list->mdx_sort_indexes[ mi ] : mdx_list->mdx_sort_indexes[ mdx_list->mdx_count - 1 - mi ];
    p = mdx_list->file_names + MDX_MAX_FILE_NAME_LEN * mi_sorted;
  }
  return p;
}

// return sorted data title pointer
unsigned char* mdx_list_get_sorted_data_title(MDX_LIST* mdx_list, int mi, int order) {
  unsigned char* p;
  if (mdx_list != NULL) {
    int mi_sorted = (order == 0) ? mdx_list->mdx_sort_indexes[ mi ] : mdx_list->mdx_sort_indexes[ mdx_list->mdx_count - 1 - mi ];
    p = mdx_list->data_titles + MDX_MAX_DATA_TITLE_LEN * mi_sorted;
  }
  return p;
}

// return sorted sub dir name pointer
unsigned char* mdx_list_get_sorted_sub_dir_name(MDX_LIST* mdx_list, int si, int order) {
  unsigned char* p;
  if (mdx_list != NULL) {
    int si_sorted = (order == 0) ? mdx_list->sub_dir_sort_indexes[ si ] : mdx_list->sub_dir_sort_indexes[ mdx_list->sub_dir_count - 1 - si ];
    p = mdx_list->sub_dir_names + MDX_MAX_FILE_NAME_LEN * si_sorted;
  }
  return p;
}

// close MDX list
void mdx_list_close(MDX_LIST* mdx_list) {
  if (mdx_list != NULL) {
    if (mdx_list->data_titles != NULL) {
      free_himem(mdx_list->data_titles, HIGH_MEMORY_USE);
      mdx_list->data_titles = NULL;
    }
    if (mdx_list->file_names != NULL) {
      free_himem(mdx_list->file_names, HIGH_MEMORY_USE);
      mdx_list->file_names = NULL;
    }
    if (mdx_list->sub_dir_names != NULL) {
      free_himem(mdx_list->sub_dir_names, HIGH_MEMORY_USE);
      mdx_list->sub_dir_names = NULL;
    }
    if (mdx_list->mdx_sort_indexes != NULL) {
      free_himem(mdx_list->mdx_sort_indexes, HIGH_MEMORY_USE);
      mdx_list->mdx_sort_indexes = NULL;
    }
    if (mdx_list->sub_dir_sort_indexes != NULL) {
      free_himem(mdx_list->sub_dir_sort_indexes, HIGH_MEMORY_USE);
      mdx_list->sub_dir_sort_indexes = NULL;
    }
    free_himem(mdx_list, 0);
    mdx_list = NULL;
  }
}