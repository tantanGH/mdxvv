#ifndef __H_PANEL__
#define __H_PANEL__

#include <stdint.h>
#include "model.h"

#define PANEL_ALL         (10)
#define PANEL_MENU        (11)

#define PANEL_CON_OPS     (20)
#define PANEL_CON_WAVE    (21)

#define PANEL_M1_OPS      (30)
#define PANEL_C1_OPS      (31)
#define PANEL_M2_OPS      (32)
#define PANEL_C2_OPS      (33)

#define PANEL_M1_ENV      (40)
#define PANEL_C1_ENV      (41)
#define PANEL_M2_ENV      (42)
#define PANEL_C2_ENV      (43)

#define PANEL_M1_WAVE     (50)
#define PANEL_C1_WAVE     (51)
#define PANEL_M2_WAVE     (52)
#define PANEL_C2_WAVE     (53)

#define PANEL_MDX_PLAY    (60)
#define PANEL_MDX_LIST    (61)

#define PANEL_MESSAGE     (70)

#define TRIM_FILE_NAME_LEN  (16)

typedef struct {
  int32_t id;
  int32_t x;
  int32_t y;
  int32_t width;
  int32_t height;
  void* scr;        // SCREEN_HANDLE* cannot be used because we have not defined it yet
  MODEL* model;     // model object tied to this panel
} PANEL;

// prototype declarations for PANEL
void panel_clear(PANEL* panel, int32_t x, int32_t y, int32_t width, int32_t height, int32_t color);
void panel_clear_all(PANEL* panel);
void panel_fill(PANEL* panel, int32_t x, int32_t y, int32_t width, int32_t height, int32_t color);
void panel_fill_all(PANEL* panel, int32_t color);
void panel_box(PANEL* panel, int32_t x, int32_t y, int32_t width, int32_t height, int32_t color);
void panel_box_all(PANEL* panel, int32_t color);
void panel_xline(PANEL* panel, int32_t x, int32_t y, int32_t len, int32_t color);
void panel_yline(PANEL* panel, int32_t x, int32_t y, int32_t len, int32_t color);

void panel_put_text(PANEL* panel, int32_t x, int32_t y, int32_t color, int32_t bold, const uint8_t* text);
void panel_put_text_center(PANEL* panel, int32_t y, int32_t color, int32_t bold, const uint8_t* text);
void panel_put_text_right(PANEL* panel, int32_t y, int32_t color, int32_t bold, const uint8_t* text);
void panel_put_text16(PANEL* panel, int32_t cx, int32_t cy, int32_t color, const uint8_t* text);

void panel_con_refresh(PANEL* panel);
void panel_op_refresh(PANEL* panel, int32_t op);
void panel_op_envelope_refresh(PANEL* panel, int32_t op);
void panel_op_waveform_refresh(PANEL* panel, int32_t op);

void panel_mdx_list_refresh(PANEL* panel);
void panel_mdx_list_up(PANEL* panel);
void panel_mdx_list_down(PANEL* panel);
void panel_mdx_list_jump(PANEL* panel, int32_t mi);

void panel_mdx_play_show_path(PANEL* panel);
void panel_mdx_play_show_title(PANEL* panel);
void panel_mdx_play_show_title2(PANEL* panel, const uint8_t* text);
void panel_mdx_play_prompt(PANEL* panel, int32_t offset, int32_t clear, const uint8_t* text);

void panel_message_show(PANEL* panel, const uint8_t* text);

#endif