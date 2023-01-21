#include "model.h"
#include "opm.h"

// initialize model
void model_init(MODEL* m) {

  m->use_high_memory = 0;

  m->voice_set = NULL;
  m->voice_index = 0;

  m->mdx_list = NULL;
  m->list_index = 0;
  m->list_view_size = LIST_VIEW_SIZE;
  m->list_view_index = 0;
  m->list_sort_order = SORT_ORDER_NORMAL;

  m->current_mdx_index = -1;
  m->current_mdx_start_time = 0;

  m->auto_play_mode = 1;
  m->auto_fadeout_time = DEFAULT_FADEOUT_TIME;

  m->message_index = 0;
  m->message_view_size = MESSAGE_VIEW_SIZE;

  OPM_WAVE* ow = &(m->opm_wave);
  ow->num_samples = ADSR_WIDTH;
  ow->available = 0;
  ow->voice = NULL;

  ow->adsr_m1 = m->adsr_m1;
  ow->adsr_c1 = m->adsr_c1;
  ow->adsr_m2 = m->adsr_m2;
  ow->adsr_c2 = m->adsr_c2;
  ow->wave_m1 = m->wave_m1;
  ow->wave_c1 = m->wave_c1;
  ow->wave_m2 = m->wave_m2;
  ow->wave_c2 = m->wave_c2;
  ow->wave_total = m->wave_total;

}
