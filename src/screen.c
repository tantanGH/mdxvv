#include <iocslib.h>
#include <doslib.h>
#include <string.h>
#include <stdint.h>

#include "screen.h"
#include "mdxvv.h"

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

// initialize panels
void screen_init_panels(SCREEN_HANDLE* scr, MODEL* model) {

  // wait vsync
  WAIT_VSYNC;
  WAIT_VBLANK;

  // full canvas
  PANEL* p = screen_get_panel(scr, PANEL_ALL);
  p->id = PANEL_ALL;
  p->x = 0;
  p->y = 0;
  p->width = 768;
  p->height = 512;
  p->scr = scr;
  p->model = model;

  panel_clear_all(p);

  // menu panel
  p = screen_get_panel(scr, PANEL_MENU);
  p->id = PANEL_MENU;
  p->x = 0;
  p->y = 0;
  p->width = 768;
  p->height = 12;
  p->scr = scr;
  p->model = model;

  panel_put_text(p, 8, 2, COLOR_DARK_PURPLE, FONT_BOLD, "E&XPORT   &QUIT   &HELP");
  //panel_put_text(p, 8, 0, COLOR_DARK_PURPLE, FONT_BOLD, "&PLAY   &STOP   E&XPORT   &QUIT   &HELP");
  //panel_put_text_right(p, 0, COLOR_DARK_PURPLE, FONT_REGULAR, "- MDXVV v" VERSION);
  panel_put_text(p, 288 + 8*1, 2, COLOR_DARK_PURPLE, FONT_BOLD, "MDXVV.X");
  //panel_put_bitmap(p, 288, 0, LOGO_WIDTH, LOGO_HEIGHT, COLOR_DARK_PURPLE, mdxvv_logo_data);
  panel_put_text(p, 288 + 8*9, 2, COLOR_DARK_PURPLE, FONT_REGULAR, "v" VERSION);
  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);

  // connection operation panel
  p = screen_get_panel(scr, PANEL_CON_OPS);
  p->id = PANEL_CON_OPS;
  p->x = 0;
  p->y = 12;
  p->width = 160;
  p->height = 254;
  p->scr = scr;
  p->model = model;

  int32_t x_ofs = 16;
  int32_t y_ofs = 24;
  int32_t y_step = 16;

  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "PROG" );

  panel_put_text(p, x_ofs, y_ofs + y_step*0, COLOR_PURPLE, FONT_BOLD, "VOICE");
  panel_put_text(p, x_ofs, y_ofs + y_step*2, COLOR_PURPLE, FONT_BOLD, "ALG");
  panel_put_text(p, x_ofs, y_ofs + y_step*3, COLOR_PURPLE, FONT_BOLD, "FL");
  panel_put_text(p, x_ofs, y_ofs + y_step*4, COLOR_PURPLE, FONT_BOLD, "OP");

  // final wave panel
  p = screen_get_panel(scr, PANEL_CON_WAVE);
  p->id = PANEL_CON_WAVE;
  p->x = 0;
  p->y = 266;
  p->width = 160;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);
  panel_xline(p, 19, 32, 114, COLOR_DARK_PURPLE);

  // operator m1 operation panel
  p = screen_get_panel(scr, PANEL_M1_OPS);
  p->id = PANEL_M1_OPS;
  p->x = 160+152*0;
  p->y = 12;
  p->width = 152;
  p->height = 200;
  p->scr = scr;
  p->model = model;

  x_ofs = 16;
  y_ofs = 24;
  y_step = 16;

  panel_box(p, -1, -1, p->width+1, p->height+1, COLOR_DARK_PURPLE);
//  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "OP1 ( M1 )" );
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "M1 / OP1" );

  panel_put_text(p, x_ofs, y_ofs + y_step*0,  COLOR_PURPLE, FONT_BOLD, "AR");
  panel_put_text(p, x_ofs, y_ofs + y_step*1,  COLOR_PURPLE, FONT_BOLD, "DR");
  panel_put_text(p, x_ofs, y_ofs + y_step*2,  COLOR_PURPLE, FONT_BOLD, "SR");
  panel_put_text(p, x_ofs, y_ofs + y_step*3,  COLOR_PURPLE, FONT_BOLD, "RR"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*4,  COLOR_PURPLE, FONT_BOLD, "SL");
  panel_put_text(p, x_ofs, y_ofs + y_step*5,  COLOR_PURPLE, FONT_BOLD, "TL");
  panel_put_text(p, x_ofs, y_ofs + y_step*6,  COLOR_PURPLE, FONT_BOLD, "KS");
  panel_put_text(p, x_ofs, y_ofs + y_step*7,  COLOR_PURPLE, FONT_BOLD, "MUL"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*8,  COLOR_PURPLE, FONT_BOLD, "DT1"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*9,  COLOR_PURPLE, FONT_BOLD, "DT2");
  panel_put_text(p, x_ofs, y_ofs + y_step*10, COLOR_PURPLE, FONT_BOLD, "AME");

  // operator c1 operation panel
  p = screen_get_panel(scr, PANEL_C1_OPS);
  p->id = PANEL_C1_OPS;
  p->x = 160+152*1;
  p->y = 12;
  p->width = 152;
  p->height = 200;
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width+1, p->height+1, COLOR_DARK_PURPLE);
//  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "OP2 ( C1 )" );
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "C1 / OP2" );
  
  panel_put_text(p, x_ofs, y_ofs + y_step*0,  COLOR_PURPLE, FONT_BOLD, "AR");
  panel_put_text(p, x_ofs, y_ofs + y_step*1,  COLOR_PURPLE, FONT_BOLD, "DR");
  panel_put_text(p, x_ofs, y_ofs + y_step*2,  COLOR_PURPLE, FONT_BOLD, "SR");
  panel_put_text(p, x_ofs, y_ofs + y_step*3,  COLOR_PURPLE, FONT_BOLD, "RR"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*4,  COLOR_PURPLE, FONT_BOLD, "SL");
  panel_put_text(p, x_ofs, y_ofs + y_step*5,  COLOR_PURPLE, FONT_BOLD, "TL");
  panel_put_text(p, x_ofs, y_ofs + y_step*6,  COLOR_PURPLE, FONT_BOLD, "KS");
  panel_put_text(p, x_ofs, y_ofs + y_step*7,  COLOR_PURPLE, FONT_BOLD, "MUL"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*8,  COLOR_PURPLE, FONT_BOLD, "DT1"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*9,  COLOR_PURPLE, FONT_BOLD, "DT2");
  panel_put_text(p, x_ofs, y_ofs + y_step*10, COLOR_PURPLE, FONT_BOLD, "AME");

  // operator m2 operation panel
  p = screen_get_panel(scr, PANEL_M2_OPS);
  p->id = PANEL_M2_OPS;
  p->x = 160+152*2;
  p->y = 12;
  p->width = 152;
  p->height = 200;
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width+1, p->height+1, COLOR_DARK_PURPLE);
//  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "OP3 ( M2 )" );
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "M2 / OP3" );
  
  panel_put_text(p, x_ofs, y_ofs + y_step*0,  COLOR_PURPLE, FONT_BOLD, "AR");
  panel_put_text(p, x_ofs, y_ofs + y_step*1,  COLOR_PURPLE, FONT_BOLD, "DR");
  panel_put_text(p, x_ofs, y_ofs + y_step*2,  COLOR_PURPLE, FONT_BOLD, "SR");
  panel_put_text(p, x_ofs, y_ofs + y_step*3,  COLOR_PURPLE, FONT_BOLD, "RR"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*4,  COLOR_PURPLE, FONT_BOLD, "SL");
  panel_put_text(p, x_ofs, y_ofs + y_step*5,  COLOR_PURPLE, FONT_BOLD, "TL");
  panel_put_text(p, x_ofs, y_ofs + y_step*6,  COLOR_PURPLE, FONT_BOLD, "KS");
  panel_put_text(p, x_ofs, y_ofs + y_step*7,  COLOR_PURPLE, FONT_BOLD, "MUL"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*8,  COLOR_PURPLE, FONT_BOLD, "DT1"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*9,  COLOR_PURPLE, FONT_BOLD, "DT2");
  panel_put_text(p, x_ofs, y_ofs + y_step*10, COLOR_PURPLE, FONT_BOLD, "AME");  

  // operator c2 operation panel
  p = screen_get_panel(scr, PANEL_C2_OPS);
  p->id = PANEL_C2_OPS;
  p->x = 160+152*3;
  p->y = 12;
  p->width = 152;
  p->height = 200;
  p->scr = scr;
  p->model = model;

  panel_yline(p, -1, -1, p->height+1, COLOR_DARK_PURPLE);
//  panel_box(p, -1, -1, p->width+1, p->height+1, COLOR_DARK_PURPLE);
//  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "OP4 ( C2 )" );
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "C2 / OP4" );

  panel_put_text(p, x_ofs, y_ofs + y_step*0,  COLOR_PURPLE, FONT_BOLD, "AR");
  panel_put_text(p, x_ofs, y_ofs + y_step*1,  COLOR_PURPLE, FONT_BOLD, "DR");
  panel_put_text(p, x_ofs, y_ofs + y_step*2,  COLOR_PURPLE, FONT_BOLD, "SR");
  panel_put_text(p, x_ofs, y_ofs + y_step*3,  COLOR_PURPLE, FONT_BOLD, "RR"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*4,  COLOR_PURPLE, FONT_BOLD, "SL");
  panel_put_text(p, x_ofs, y_ofs + y_step*5,  COLOR_PURPLE, FONT_BOLD, "TL");
  panel_put_text(p, x_ofs, y_ofs + y_step*6,  COLOR_PURPLE, FONT_BOLD, "KS");
  panel_put_text(p, x_ofs, y_ofs + y_step*7,  COLOR_PURPLE, FONT_BOLD, "MUL"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*8,  COLOR_PURPLE, FONT_BOLD, "DT1"); 
  panel_put_text(p, x_ofs, y_ofs + y_step*9,  COLOR_PURPLE, FONT_BOLD, "DT2");
  panel_put_text(p, x_ofs, y_ofs + y_step*10, COLOR_PURPLE, FONT_BOLD, "AME");      

  // operator m1 envelope panel
  p = screen_get_panel(scr, PANEL_M1_ENV);
  p->id = PANEL_M1_ENV;
  p->x = 160+152*0;
  p->y = 212;
  p->width = 152;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "ENVELOPE" );
  panel_xline(p, 19, 16+35, 114, COLOR_DARK_PURPLE);
  panel_yline(p, 19, 10, 41, COLOR_DARK_PURPLE);

  // operator c1 envelope panel
  p = screen_get_panel(scr, PANEL_C1_ENV);
  p->id = PANEL_C1_ENV;
  p->x = 160+152*1;
  p->y = 212;
  p->width = 152;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "ENVELOPE" );
  panel_xline(p, 19, 16+35, 114, COLOR_DARK_PURPLE);
  panel_yline(p, 19, 10, 41, COLOR_DARK_PURPLE);

  // operator m2 envelope panel
  p = screen_get_panel(scr, PANEL_M2_ENV);
  p->id = PANEL_M2_ENV;
  p->x = 160+152*2;
  p->y = 212;
  p->width = 152;
  p->height = 54; 
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "ENVELOPE" );
  panel_xline(p, 19, 16+35, 114, COLOR_DARK_PURPLE);
  panel_yline(p, 19, 10, 41, COLOR_DARK_PURPLE);

  // operator c2 envelope panel
  p = screen_get_panel(scr, PANEL_C2_ENV);
  p->id = PANEL_C2_ENV;
  p->x = 160+152*3;
  p->y = 212;
  p->width = 152;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  //panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "ENVELOPE" );
  panel_xline(p, 19, 16+35, 114, COLOR_DARK_PURPLE);
  panel_yline(p, 19, 10, 41, COLOR_DARK_PURPLE);
  panel_xline(p, -1, -1, p->width+1, COLOR_DARK_PURPLE);
  panel_yline(p, -1, -1, p->height+1, COLOR_DARK_PURPLE);

  // operator m1 wave panel
  p = screen_get_panel(scr, PANEL_M1_WAVE);
  p->id = PANEL_M1_WAVE;
  p->x = 160+152*0;
  p->y = 266;
  p->width = 152;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
  panel_xline(p, 19, 32, 114, COLOR_DARK_PURPLE);

  // operator c1 wave panel
  p = screen_get_panel(scr, PANEL_C1_WAVE);
  p->id = PANEL_C1_WAVE;
  p->x = 160+152*1;
  p->y = 266;
  p->width = 152;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
  panel_xline(p, 19, 32, 114, COLOR_DARK_PURPLE);

  // operator m2 wave panel
  p = screen_get_panel(scr, PANEL_M2_WAVE);
  p->id = PANEL_M2_WAVE;
  p->x = 160+152*2;
  p->y = 266;
  p->width = 152;
  p->height = 54; 
  p->scr = scr;
  p->model = model;

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
  panel_xline(p, 19, 32, 114, COLOR_DARK_PURPLE);

  // operator c2 wave panel
  p = screen_get_panel(scr, PANEL_C2_WAVE);
  p->id = PANEL_C2_WAVE;
  p->x = 160+152*3;
  p->y = 266;
  p->width = 152;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  //panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
  panel_xline(p, 19, 32, 114, COLOR_DARK_PURPLE);
  panel_xline(p, -1, -1, p->width+1, COLOR_DARK_PURPLE);
  panel_yline(p, -1, -1, p->height+1, COLOR_DARK_PURPLE);
  panel_xline(p, -1, p->height-1, p->width+1, COLOR_DARK_PURPLE);

  // mdx play panel
  p = screen_get_panel(scr, PANEL_MDX_PLAY);
  p->id = PANEL_MDX_PLAY;
  p->x = 0;
  p->y = 320;
  p->width = 768;
  p->height = 22;
  p->scr = scr;
  p->model = model;

  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);

  // mdx list panel
  p = screen_get_panel(scr, PANEL_MDX_LIST);
  p->id = PANEL_MDX_LIST;
  p->x = 0;
  p->y = 342;
  p->width = 768;
  p->height = 114;
  p->scr = scr;
  p->model = model;

  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);

  // message panel
  p = screen_get_panel(scr, PANEL_MESSAGE);
  p->id = PANEL_MESSAGE;
  p->x = 0;
  p->y = 456;
  p->width = 768;
  p->height = 56;
  p->scr = scr;
  p->model = model;

  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);  
}
