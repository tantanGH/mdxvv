#include <iocslib.h>
#include <doslib.h>
#include <string.h>
#include <stdint.h>

#include "screen.h"

static void clear_text_screen() {
  struct TXFILLPTR fill = { 0, 0, 0, 768, 512, 0 };
  for (int32_t i = 0; i < 4; i++) {
    fill.vram_page = i;
    TXFILL(&fill);  
  }
}

// clear and initialize screen and sprites
void screen_init(SCREEN_HANDLE* scr, int32_t preserve_graphic) {

  if (preserve_graphic) {
    // text clear only
    clear_text_screen();
  } else {
    CRTMOD(16);
    G_CLR_ON();
  }

  scr->original_fnk_mode = C_FNKMOD(-1);
  C_FNKMOD(3);
  C_CUROFF();
  //B_CUROFF();

  // preserve original text palette
  for (int32_t i = 0; i < 4; i++) {
    scr->original_tpalette[i] = TPALET(i,-1);
  }

  // set text palette
  TPALET(0, PALETTE_COLOR0);
  TPALET(1, PALETTE_COLOR1);
  TPALET(2, PALETTE_COLOR2);
  TPALET(3, PALETTE_COLOR3);
}

// reset screen
void screen_reset(SCREEN_HANDLE* scr, int32_t preserve_graphic) {

  if (preserve_graphic) {
    clear_text_screen();
  } else {
    CRTMOD(16);
    G_CLR_ON();
  }

  C_FNKMOD(scr->original_fnk_mode);
  C_CURON();
  //B_CURON();

  // resume text palette
  for (int32_t i = 0; i < 4; i++) {
    TPALET(i, scr->original_tpalette[i]);
  }   
}

// prepare 8x8 regular and bold font data
void screen_init_font(SCREEN_HANDLE* scr) {

  for (int32_t i = 0; i < 256; i++) {

    // 8x8 regular font
    scr->font_data_8x8[i].xl = 8;
    scr->font_data_8x8[i].yl = 8;
    memcpy(scr->font_data_8x8[i].buffer, FONT_ADDR_8x8 + FONT_BYTES_8x8 * i, FONT_BYTES_8x8);

    // 8x8 bold font
    scr->font_data_8x8_bold[i].xl = 8;
    scr->font_data_8x8_bold[i].yl = 8;
    memcpy(scr->font_data_8x8_bold[i].buffer, FONT_ADDR_8x8 + FONT_BYTES_8x8 * i, FONT_BYTES_8x8);
    for (int32_t j = 0; j < FONT_BYTES_8x8; j++) {
      scr->font_data_8x8_bold[i].buffer[j] |= ( scr->font_data_8x8_bold[i].buffer[j] >> 1 ) & 0xff;
    }

  }
}

// obtain panel positions
PANEL* screen_get_panel(SCREEN_HANDLE* scr, int32_t panel_id) {

  PANEL* panel = NULL;

  switch (panel_id) {
    case PANEL_ALL:
      panel = &(scr->panel_all);
      break;
    case PANEL_MENU:
      panel = &(scr->panel_menu);
      break;

    case PANEL_CON_OPS:
      panel = &(scr->panel_con_ops);
      break;
    case PANEL_CON_WAVE:
      panel = &(scr->panel_con_wave);
      break;

    case PANEL_M1_OPS:
      panel = &(scr->panel_m1_ops);
      break;
    case PANEL_C1_OPS:
      panel = &(scr->panel_c1_ops);
      break;
    case PANEL_M2_OPS:
      panel = &(scr->panel_m2_ops);
      break;
    case PANEL_C2_OPS:
      panel = &(scr->panel_c2_ops);
      break;

    case PANEL_M1_ENV:
      panel = &(scr->panel_m1_env);
      break;
    case PANEL_C1_ENV:
      panel = &(scr->panel_c1_env);
      break;
    case PANEL_M2_ENV:
      panel = &(scr->panel_m2_env);
      break;
    case PANEL_C2_ENV:
      panel = &(scr->panel_c2_env);
      break;

    case PANEL_M1_WAVE:
      panel = &(scr->panel_m1_wave);
      break;
    case PANEL_C1_WAVE:
      panel = &(scr->panel_c1_wave);
      break;
    case PANEL_M2_WAVE:
      panel = &(scr->panel_m2_wave);
      break;
    case PANEL_C2_WAVE:
      panel = &(scr->panel_c2_wave);
      break;

    case PANEL_MDX_PLAY:
      panel = &(scr->panel_mdx_play);
      break;
    case PANEL_MDX_LIST:
      panel = &(scr->panel_mdx_list);
      break;

    case PANEL_MESSAGE:
      panel = &(scr->panel_message);
      break;
  }

  return panel;
}
