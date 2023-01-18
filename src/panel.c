#include <stddef.h>
#include <string.h>
#include <time.h>
#include <doslib.h>
#include <iocslib.h>

#include "panel.h"
#include "screen.h"
#include "voice.h"
#include "diagram.h"

// clear
void panel_clear(PANEL* panel, int x, int y, int width, int height, int color) {
  if (panel != NULL) {
    struct TXFILLPTR fill = { 0, panel->x + x, panel->y + y, width, height, 0 };
    if (color & 0x01) {
      fill.vram_page = 0;
      TXFILL(&fill);  
    }
    if (color & 0x02) {
      fill.vram_page = 1;
      TXFILL(&fill);        
    }
  }
}

// clear all
void panel_clear_all(PANEL* panel) {
  if (panel != NULL) {
    panel_clear(panel, 0, 0, panel->width, panel->height, 0x03);
  }
}

// fill
void panel_fill(PANEL* panel, int x, int y, int width, int height, int color) {
  if (panel != NULL) {
    struct TXFILLPTR fill = { 0, panel->x + x, panel->y + y, width, height, 0xffff };
    if (color & 0x01) {
      fill.vram_page = 0;
      TXFILL(&fill);  
    }
    if (color & 0x02) {
      fill.vram_page = 1;
      TXFILL(&fill);
    }
  }
}

// fill all
void panel_fill_all(PANEL* panel, int color) {
  if (panel != NULL) {
    panel_fill(panel, 0, 0, panel->width, panel->height, color);
  }
}

// box 
void panel_box(PANEL* panel, int x, int y, int width, int height, int color) {
  if (panel != NULL) {
    struct TBOXPTR box = { 0, panel->x + x, panel->y + y, width, height, 0xffff };
    if (color & 0x01) {
      box.vram_page = 0;
      TXBOX(&box);
    }
    if (color & 0x02) {
      box.vram_page = 1;
      TXBOX(&box);
    }
  }
}

// box all
void panel_box_all(PANEL* panel, int color) {
  if (panel != NULL) {
    panel_box(panel, 0, 0, panel->width, panel->height, color);
  }
}

// x line
void panel_xline(PANEL* panel, int x, int y, int len, int color) {
  if (panel != NULL) {
    struct XLINEPTR xlp = { 0, panel->x + x, panel->y + y, len, 0xffff };
    if (color & 0x01) {
      xlp.vram_page = 0;
      TXXLINE(&xlp);
    }
    if (color & 0x02) {
      xlp.vram_page = 1;
      TXXLINE(&xlp);
    }
  }
}

// y line
void panel_yline(PANEL* panel, int x, int y, int len, int color) {
  if (panel != NULL) {
    struct YLINEPTR ylp = { 0, panel->x + x, panel->y + y, len, 0xffff };
    if (color & 0x01) {
      ylp.vram_page = 0;
      TXYLINE(&ylp);
    }
    if (color & 0x02) {
      ylp.vram_page = 1;
      TXYLINE(&ylp);
    }
  }
}

// put text in 8x8 font
void panel_put_text(PANEL* panel, int x, int y, int color, int bold, const unsigned char* text) {

  if (panel == NULL) return;

  SCREEN_HANDLE* scr = (SCREEN_HANDLE*)(panel->scr);
  int len = strlen(text);
  int ofs = 0;
  for (int i = 0; i < len; i++) {
    int fx = x + 8*ofs;
    if (text[i] == '&') {   // underscore
      panel_xline(panel, fx, y+8, 8, color);
      continue;
    }
    if (color & 0x01) {
      TCOLOR(1);
      if (bold == FONT_BOLD) {
        TEXTPUT(panel->x + fx, panel->y + y, &scr->font_data_8x8_bold[text[i]]);
      } else {
        TEXTPUT(panel->x + fx, panel->y + y, &scr->font_data_8x8[text[i]]);
      }
    }
    if (color & 0x02) {
      TCOLOR(2);
      if (bold == FONT_BOLD) {
        TEXTPUT(panel->x + fx, panel->y + y, &scr->font_data_8x8_bold[text[i]]);    
      } else {
        TEXTPUT(panel->x + fx, panel->y + y, &scr->font_data_8x8[text[i]]);    
      }
    }
    ofs++;
  }
}

// put text in 8x8 bold font with centering
void panel_put_text_center(PANEL* panel, int y, int color, int bold, const unsigned char* text) {
  int len = strlen(text);
  int fx = (panel->width - 8 * len)/2;
  int fy = y;
  panel_put_text(panel, fx, fy, color, bold, text);
}

// put text in 8x8 bold font with right alignment
void panel_put_text_right(PANEL* panel, int y, int color, int bold, const unsigned char* text) {
  int len = strlen(text);
  int fx = panel->width - 8 * len;
  int fy = y;
  panel_put_text(panel, fx, fy, color, bold, text);
}

// put text in 8x16/16x16 font
void panel_put_text16(PANEL* panel, int x, int y, int color, const unsigned char* text) {

  if (panel == NULL) return;

  SCREEN_HANDLE* scr = (SCREEN_HANDLE*)(panel->scr);
  int len = strlen(text);
  int fy = panel->y + y;

  for (int i = 0; i < len; i++) {

    int fx = panel->x + x + 8*i;

    // SJIS code?
    unsigned short code = text[i];
    if (i < len-1 && (text[i] >= 0x81 && text[i] <= 0x9f) || (text[i] >= 0xe0 && text[i] <= 0xfc)) {
      code = text[i] * 256 + text[i+1];
      i++;
    }

    struct REGS in_regs = { 0 };
    struct REGS out_regs = { 0 };

    in_regs.d0 = 0x16;      // IOCS _FNTADR .. this IOCS call is not available in XC library
    in_regs.d1 = code;
    in_regs.d2 = 8;

    TRAP15(&in_regs, &out_regs);

    struct FNTBUF fb;
    fb.xl = ( out_regs.d1 + 1 ) * 8;
    fb.yl = out_regs.d2 + 1;
    memcpy(&(fb.buffer[0]), (void*)(out_regs.d0), fb.xl * fb.yl / 8);

    if (color & 0x01) {
      TCOLOR(1);
      TEXTPUT(fx, fy, &fb);
    }
    if (color & 0x02) {
      TCOLOR(2);
      TEXTPUT(fx, fy, &fb);
    }
  }
}

// connection panel refresh with the latest model
void panel_con_refresh(PANEL* panel) {

  if (panel == NULL) goto exit;
  
  MODEL* m = panel->model;
  VOICE_SET* vs = m->voice_set;

  if (vs == NULL) {
    panel_clear(panel, 56, 24, panel->width - 60, panel->height - 28, COLOR_PURPLE);
    goto exit;
  }

  VOICE* v = &(vs->voices[ m->voice_index ]);

  int x_ofs2 = 56;
  int y_ofs1 = 24;
  int y_ofs2 = 132;
  int y_step = 16;

  static char voice_no_str[]    = "    ";
  static char connection_str[]  = "   ";
  static char feedback_str[]    = "   ";
  static char slot_mask_str[]   = "   ";

  sprintf(voice_no_str,   "@%d   ", v->voice_id  );
  sprintf(connection_str, "%3d",    v->connection);
  sprintf(feedback_str,   "%3d",    v->feedback  );
  sprintf(slot_mask_str,  "%3d",    v->slot_mask );

  panel_put_text(panel, x_ofs2 + 16, y_ofs1 + y_step*0, COLOR_WHITE, FONT_BOLD, voice_no_str  );
  panel_put_text(panel, x_ofs2,      y_ofs1 + y_step*2, COLOR_WHITE, FONT_REGULAR, connection_str);
  panel_put_text(panel, x_ofs2,      y_ofs1 + y_step*3, COLOR_WHITE, FONT_REGULAR, feedback_str  );
  panel_put_text(panel, x_ofs2,      y_ofs1 + y_step*4, COLOR_WHITE, FONT_REGULAR, slot_mask_str );
/*
  panel_put_text(panel, x_ofs2,      y_ofs1 + y_step*2, COLOR_PURPLE, FONT_REGULAR, connection_str);
  panel_put_text(panel, x_ofs2,      y_ofs1 + y_step*3, COLOR_PURPLE, FONT_REGULAR, feedback_str  );
  panel_put_text(panel, x_ofs2,      y_ofs1 + y_step*4, COLOR_PURPLE, FONT_REGULAR, slot_mask_str );
*/
  int bar_x1 = 96;
  int bar_x2 = 132;

  for (int i = 0; i < 3; i++) {
    int pct;
    switch (i) {
      case  0: pct = (bar_x2 - bar_x1) * v->connection / MAX_CONNECTION; break;
      case  1: pct = (bar_x2 - bar_x1) * v->feedback   / MAX_FEEDBACK;   break;
      case  2: pct = (bar_x2 - bar_x1) * v->slot_mask  / MAX_SLOT_MASK;  break;
    }
    panel_clear(panel, bar_x1, y_ofs1 + y_step*(i+2), bar_x2 - bar_x1 + 1 + 2, 8, COLOR_DARK_PURPLE);
    panel_xline(panel, bar_x1, y_ofs1 + y_step*(i+2) +4, bar_x2 - bar_x1 + 1 + 2, COLOR_DARK_PURPLE);
    panel_fill(panel, bar_x1 + pct, y_ofs1 + y_step*(i+2), 3, 8, COLOR_DARK_PURPLE);
  }

  //panel_con_show_diagram(panel, 0, y_ofs2, COLOR_PURPLE, v->connection);
  unsigned short* d = NULL;
  switch (v->connection) {
    case 0: d = diagram_con0_data; break;
    case 1: d = diagram_con1_data; break;
    case 2: d = diagram_con2_data; break;
    case 3: d = diagram_con3_data; break;
    case 4: d = diagram_con4_data; break;
    case 5: d = diagram_con5_data; break;
    case 6: d = diagram_con6_data; break;
    case 7: d = diagram_con7_data; break;
  }

  for (int i = 0; i < DIAGRAM_HEIGHT; i++) {
    int ofs = ( panel->y + y_ofs2 + i ) * 1024 / 16 + ( panel->x + 0 ) / 16;
    for (int j = 0; j < DIAGRAM_WIDTH / 16; j++) {
      TVRAM_PAGE0[ ofs++ ] = *d++;
    }
  }
  panel_yline(panel, DIAGRAM_WIDTH - 1, y_ofs2, DIAGRAM_HEIGHT, COLOR_DARK_PURPLE);   // woraround... should be removed later

exit:
}

// refresh operator panel with the latest model content
void panel_op_refresh(PANEL* panel, int op) {

  if (panel == NULL) goto exit;

  MODEL* m = panel->model;
  VOICE_SET* vs = m->voice_set;
  
  if (vs == NULL) {
    panel_clear(panel, 56, 24, panel->width - 60, panel->height - 28, COLOR_PURPLE);
    goto exit;
  }

  VOICE* v = &(vs->voices[ m->voice_index ]);

  unsigned char attack_rate;
  unsigned char decay_rate1;
  unsigned char decay_rate2;
  unsigned char release_rate;
  unsigned char decay_level1;
  unsigned char total_level;
  unsigned char key_scaling;
  unsigned char phase_multi;
  unsigned char detune1;
  unsigned char detune2;
  unsigned char ams_enable;

  switch (op) {
    case 1:
      attack_rate  = v->attack_rate_m1;
      decay_rate1  = v->decay_rate1_m1;
      decay_rate2  = v->decay_rate2_m1;
      release_rate = v->release_rate_m1;
      decay_level1 = v->decay_level1_m1;
      total_level  = v->total_level_m1;
      key_scaling  = v->key_scaling_m1;
      phase_multi  = v->phase_multi_m1;
      detune1      = v->detune1_m1;
      detune2      = v->detune2_m1;
      ams_enable   = v->ams_enable_m1;
      break;

    case 2:
      attack_rate  = v->attack_rate_c1;
      decay_rate1  = v->decay_rate1_c1;
      decay_rate2  = v->decay_rate2_c1;
      release_rate = v->release_rate_c1;
      decay_level1 = v->decay_level1_c1;
      total_level  = v->total_level_c1;
      key_scaling  = v->key_scaling_c1;
      phase_multi  = v->phase_multi_c1;
      detune1      = v->detune1_c1;
      detune2      = v->detune2_c1;
      ams_enable   = v->ams_enable_c1;
      break;

    case 3:
      attack_rate  = v->attack_rate_m2;
      decay_rate1  = v->decay_rate1_m2;
      decay_rate2  = v->decay_rate2_m2;
      release_rate = v->release_rate_m2;
      decay_level1 = v->decay_level1_m2;
      total_level  = v->total_level_m2;
      key_scaling  = v->key_scaling_m2;
      phase_multi  = v->phase_multi_m2;
      detune1      = v->detune1_m2;
      detune2      = v->detune2_m2;
      ams_enable   = v->ams_enable_m2;    
      break;

    case 4:
      attack_rate  = v->attack_rate_c2;
      decay_rate1  = v->decay_rate1_c2;
      decay_rate2  = v->decay_rate2_c2;
      release_rate = v->release_rate_c2;
      decay_level1 = v->decay_level1_c2;
      total_level  = v->total_level_c2;
      key_scaling  = v->key_scaling_c2;
      phase_multi  = v->phase_multi_c2;
      detune1      = v->detune1_c2;
      detune2      = v->detune2_c2;
      ams_enable   = v->ams_enable_c2;
      break;
    
    default:
      goto exit;
  }

  int x_ofs2 = 56;
  int y_ofs1 = 24;
  int y_step = 16;

  static char attack_rate_str[]  = "   ";
  static char decay_rate1_str[]  = "   ";
  static char decay_rate2_str[]  = "   ";
  static char release_rate_str[] = "   ";
  static char decay_level1_str[] = "   ";
  static char total_level_str[]  = "   ";
  static char key_scaling_str[]  = "   ";
  static char phase_multi_str[]  = "   ";
  static char detune1_str[]      = "   ";
  static char detune2_str[]      = "   ";
  static char ams_enable_str[]   = "   ";

  sprintf(attack_rate_str,  "%3d", attack_rate);
  sprintf(decay_rate1_str,  "%3d", decay_rate1);
  sprintf(decay_rate2_str,  "%3d", decay_rate2);
  sprintf(release_rate_str, "%3d", release_rate);
  sprintf(decay_level1_str, "%3d", decay_level1);
  sprintf(total_level_str,  "%3d", total_level);
  sprintf(key_scaling_str,  "%3d", key_scaling);
  sprintf(phase_multi_str,  "%3d", phase_multi);
  sprintf(detune1_str,      "%3d", detune1);
  sprintf(detune2_str,      "%3d", detune2);
  sprintf(ams_enable_str,   "%3d", ams_enable);

  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*0,  COLOR_WHITE, FONT_REGULAR, attack_rate_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*1,  COLOR_WHITE, FONT_REGULAR, decay_rate1_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*2,  COLOR_WHITE, FONT_REGULAR, decay_rate2_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*3,  COLOR_WHITE, FONT_REGULAR, release_rate_str); 
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*4,  COLOR_WHITE, FONT_REGULAR, decay_level1_str);
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*5,  COLOR_WHITE, FONT_REGULAR, total_level_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*6,  COLOR_WHITE, FONT_REGULAR, key_scaling_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*7,  COLOR_WHITE, FONT_REGULAR, phase_multi_str ); 
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*8,  COLOR_WHITE, FONT_REGULAR, detune1_str     ); 
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*9,  COLOR_WHITE, FONT_REGULAR, detune2_str     );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*10, COLOR_WHITE, FONT_REGULAR, ams_enable_str  );      

/*
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*0,  COLOR_PURPLE, FONT_REGULAR, attack_rate_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*1,  COLOR_PURPLE, FONT_REGULAR, decay_rate1_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*2,  COLOR_PURPLE, FONT_REGULAR, decay_rate2_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*3,  COLOR_PURPLE, FONT_REGULAR, release_rate_str); 
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*4,  COLOR_PURPLE, FONT_REGULAR, decay_level1_str);
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*5,  COLOR_PURPLE, FONT_REGULAR, total_level_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*6,  COLOR_PURPLE, FONT_REGULAR, key_scaling_str );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*7,  COLOR_PURPLE, FONT_REGULAR, phase_multi_str ); 
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*8,  COLOR_PURPLE, FONT_REGULAR, detune1_str     ); 
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*9,  COLOR_PURPLE, FONT_REGULAR, detune2_str     );
  panel_put_text(panel, x_ofs2, y_ofs1 + y_step*10, COLOR_PURPLE, FONT_REGULAR, ams_enable_str  );        
*/
  int bar_x1 = 96;
  int bar_x2 = 132;

  for (int i = 0; i < 11; i++) {
    int pct;
    switch (i) {
      case  0: pct = (bar_x2 - bar_x1) * attack_rate  / MAX_ATTACK_RATE;  break;
      case  1: pct = (bar_x2 - bar_x1) * decay_rate1  / MAX_DECAY_RATE1;  break;
      case  2: pct = (bar_x2 - bar_x1) * decay_rate2  / MAX_DECAY_RATE2;  break;
      case  3: pct = (bar_x2 - bar_x1) * release_rate / MAX_RELEASE_RATE; break;
      case  4: pct = (bar_x2 - bar_x1) * decay_level1 / MAX_DECAY_LEVEL1; break;
      case  5: pct = (bar_x2 - bar_x1) * total_level  / MAX_TOTAL_LEVEL;  break;
      case  6: pct = (bar_x2 - bar_x1) * key_scaling  / MAX_KEY_SCALING;  break;
      case  7: pct = (bar_x2 - bar_x1) * phase_multi  / MAX_PHASE_MULTI;  break;
      case  8: pct = (bar_x2 - bar_x1) * detune1      / MAX_DETUNE1;      break;
      case  9: pct = (bar_x2 - bar_x1) * detune2      / MAX_DETUNE2;      break;
      case 10: pct = (bar_x2 - bar_x1) * ams_enable   / MAX_AMS_ENABLE;   break;
    }
    panel_clear(panel, bar_x1, y_ofs1 + y_step*i, bar_x2 - bar_x1 + 1 + 2, 8, COLOR_DARK_PURPLE);
    panel_xline(panel, bar_x1, y_ofs1 + y_step*i + 4, bar_x2 - bar_x1 + 1 + 2, COLOR_DARK_PURPLE);
    panel_fill(panel, bar_x1 + pct, y_ofs1 + y_step*i + 0, 3, 8, COLOR_DARK_PURPLE);
  }

  exit:
}

// refresh operator envelope panel with the latest model content
void panel_op_envelope_refresh(PANEL* panel, int op) {
  if (panel != NULL) {  
    MODEL* m = panel->model;
    float* adsr = NULL;
    switch (op) {
      case 1: adsr = m->adsr_m1; break;
      case 2: adsr = m->adsr_c1; break;
      case 3: adsr = m->adsr_m2; break;
      case 4: adsr = m->adsr_c2; break;
    }

    if (adsr != NULL) {

      static unsigned short frame_buffer[ 40 ][ ADSR_WIDTH/16 + 1 ];
      for (int i = 0; i < 40; i++) {
        for (int j = 0; j < ADSR_WIDTH/16 + 1; j++) {
          frame_buffer[i][j] = 0;
        }
      }

      int x0 = panel->x + 20;
      int x0a = x0 >> 4;

      for (int i = 0; i < ADSR_WIDTH - 1; i++) {

        int x1 = panel->x + 20 + i;
        int x1a = x1 >> 4;
        unsigned short x1b = 0x8000 >> (x1 % 16);

        int y1 = 40 * ( 1.0 - adsr[i]   );
        int y2 = 40 * ( 1.0 - adsr[i+1] );

        if (y1 > y2) {
          for (int y = y1; y > y2; y--) {
            frame_buffer[ y ][ x1a - x0a ] |= x1b;
          }
        } else if (y1 < y2) {
          for (int y = y1; y < y2; y++) {
            frame_buffer[ y ][ x1a - x0a ] |= x1b;
          }
        } else {
          frame_buffer[ y1 ][ x1a - x0a ] |= x1b;
        }

      }

      for (int i = 0; i < 40; i++) {
        for (int j = 0; j < ADSR_WIDTH/16 + 1; j++) {
          int x1 = panel->x + 20 + j * 16;
          int x1a = x1 >> 4;
          TVRAM_PAGE0[ ( panel->y + 11 + i ) * 1024/16 + x1a ] = frame_buffer[i][j];
        }
      }

    }

/*
    panel_clear(panel, 20, 11, ADSR_WIDTH, 40, COLOR_WHITE);
    if (adsr != NULL) {
      for (int i = 0; i < ADSR_WIDTH - 1; i++) {
        int x0 = panel->x + 20 + i;
        int y0 = panel->y + 11 + 40 * ( 1.0 - adsr[i]   );
        int y1 = panel->y + 11 + 40 * ( 1.0 - adsr[i+1] );
        int x0a = x0 >> 4;
        unsigned short x0b = 0x8000 >> (x0 % 16);
        if (y0 > y1) {
          for (int y = y0; y > y1; y--) {
            TVRAM_PAGE0[ y * 64 + x0a ] |= x0b;
          }
        } else if (y0 < y1) {
          for (int y = y0; y < y1; y++) {
            TVRAM_PAGE0[ y * 64 + x0a ] |= x0b;
          }
        } else {
          TVRAM_PAGE0[ y0 * 64 + x0a ] |= x0b;
        }
      }
    }
*/
  }
}

// refresh operator waveform panel with the latest model content
void panel_op_waveform_refresh(PANEL* panel, int op) {

  if (panel == NULL) goto exit;
  
  MODEL* m = panel->model;
  VOICE_SET* vs = m->voice_set;

  if (vs == NULL) {
    panel_clear(panel, 56, 24, panel->width - 60, panel->height - 28, COLOR_PURPLE);
    goto exit;
  }

  VOICE* v = &(vs->voices[ m->voice_index ]);

  unsigned char key_scaling;
  unsigned char phase_multi;
  unsigned char detune1;
  unsigned char detune2;
  unsigned char ams_enable;

  switch (op) {
    case 1:
      key_scaling  = v->key_scaling_m1;
      phase_multi  = v->phase_multi_m1;
      detune1      = v->detune1_m1;
      detune2      = v->detune2_m1;
      ams_enable   = v->ams_enable_m1;
      break;

    case 2:
      key_scaling  = v->key_scaling_c1;
      phase_multi  = v->phase_multi_c1;
      detune1      = v->detune1_c1;
      detune2      = v->detune2_c1;
      ams_enable   = v->ams_enable_c1;
      break;

    case 3:
      key_scaling  = v->key_scaling_m2;
      phase_multi  = v->phase_multi_m2;
      detune1      = v->detune1_m2;
      detune2      = v->detune2_m2;
      ams_enable   = v->ams_enable_m2;    
      break;

    case 4:
      key_scaling  = v->key_scaling_c2;
      phase_multi  = v->phase_multi_c2;
      detune1      = v->detune1_c2;
      detune2      = v->detune2_c2;
      ams_enable   = v->ams_enable_c2;
      break;
    
    default:
      goto exit;
  }

  exit:
}

// show message with scroll up
void panel_message_show(PANEL* panel, const unsigned char *message) {

  if (panel == NULL || message == NULL) return;

  WAIT_VSYNC;
  WAIT_VBLANK;

  MODEL* m = panel->model;

  if (m->message_index == m->message_view_size) {
    // scroll up
    for (int i = 0; i < 10 * (m->message_view_size - 1); i++) {
      for (int j = 0; j < panel->width / 16; j++) {
        TVRAM_PAGE0[ ( panel->y + 4 + i ) * 1024 / 16 + panel->x / 16 + j ]
         = TVRAM_PAGE0[ ( panel->y + 4 + i + 10 ) * 1024 / 16 + panel->x / 16 + j ];
        TVRAM_PAGE1[ ( panel->y + 4 + i ) * 1024 / 16 + panel->x / 16 + j ]
         = TVRAM_PAGE1[ ( panel->y + 4 + i + 10 ) * 1024 / 16 + panel->x / 16 + j ];
      }
    }
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < panel->width / 16; j++) {
        TVRAM_PAGE0[ ( panel->y + 4 + ( m->message_view_size - 1 ) * 10 + i ) * 1024 / 16 + j ] = 0x0000;
        TVRAM_PAGE1[ ( panel->y + 4 + ( m->message_view_size - 1 ) * 10 + i ) * 1024 / 16 + j ] = 0x0000;
      }
    }
    m->message_index--;
  }

  time_t tm = time(NULL);
  static char line[128];
  strftime(line, 128, "[%Y-%m-%d %H:%M:%S] ", localtime(&tm));
  strcat(line, message);
  
  panel_put_text(panel, 4, 4 + 10 * m->message_index, COLOR_DARK_PURPLE, FONT_REGULAR, line);
  m->message_index++;
}

// trim long file name
static void trim_file_name(unsigned char* buffer, int buflen, const unsigned char* file_name) {
  if (strlen(file_name) <= buflen) {
    strcpy(buffer, file_name);
    return;
  }
  memcpy(buffer, file_name, buflen-3);
  buffer[buflen-3] = '.';
  buffer[buflen-2] = '.';
  buffer[buflen-1] = '\0';
}

// update mdx list
void panel_mdx_list_refresh(PANEL* panel) {

  if (panel == NULL) return;

  WAIT_VSYNC;
  WAIT_VBLANK;

  MODEL* m = panel->model;
  MDX_LIST* mdx_list = m->mdx_list;
  int yofs = 0;

  panel_clear(panel, 4, 0, 760, 16*m->list_view_size, COLOR_WHITE | COLOR_DARK_PURPLE);

  // list sub directories first
  for (int i = 0; i < mdx_list->sub_dir_count && yofs < m->list_view_size; i++) {
    int si = i ;
    panel_put_text(panel, 4, 16 * yofs + 7, COLOR_DARK_PURPLE, FONT_REGULAR, "<DIR>");
    panel_put_text16(panel, 4 + 8*16, 16 * yofs, COLOR_DARK_PURPLE, mdx_list_get_sorted_sub_dir_name(mdx_list, si, m->list_sort_order));
    yofs++;
  }

  // list MDX files
  for (int i = 0; i < mdx_list->mdx_count && yofs < m->list_view_size; i++) {
    static unsigned char trimmed_file_name [ TRIM_FILE_NAME_LEN ];
    int mi = i ;
    trim_file_name(trimmed_file_name, TRIM_FILE_NAME_LEN, mdx_list_get_sorted_file_name(mdx_list, mi, m->list_sort_order));
    panel_put_text(panel, 4, 16 * yofs + 7, COLOR_DARK_PURPLE, FONT_REGULAR, trimmed_file_name);
    panel_put_text16(panel, 4 + 8*16, 16 * yofs, COLOR_PURPLE, mdx_list_get_sorted_data_title(mdx_list, mi, m->list_sort_order));
    yofs++;
  }

  // cursor bar
  if (mdx_list->sub_dir_count + mdx_list->mdx_count > 0) {
    panel_xline(panel, 4, 16 * m->list_index + 15, 760, COLOR_WHITE);
  }
}

// move cursor bar up in the list panel
void panel_mdx_list_up(PANEL* panel) {

  if (panel == NULL) return;

  MODEL* m = panel->model;
  MDX_LIST* mdx_list = m->mdx_list;

  if (mdx_list == NULL || (mdx_list->sub_dir_count + mdx_list->mdx_count) == 0) return;

  WAIT_VSYNC;
  WAIT_VBLANK;

  if (m->list_index > 0) {

    // just move cursor up
    panel_clear(panel, 4, 16 * m->list_index + 15, 760, 1, COLOR_WHITE);    // erase cursor
    m->list_index--;
    panel_xline(panel, 4, 16 * m->list_index + 15, 760, COLOR_WHITE);

  } else if (m->list_view_index > 0) {

    // we are already at the top of the list, scrolling down
    panel_clear(panel, 4, 16 * m->list_index + 15, 760, 1, COLOR_WHITE);    // erase cursor
    for (int i = 16 * m->list_view_size - 1; i >= 16; i--) {
      for (int j = 0; j < panel->width / 16; j++) {
        TVRAM_PAGE0[ ( panel->y + i ) * 1024 / 16 + panel->x / 16 + j ]
         = TVRAM_PAGE0[ ( panel->y + i - 16 ) * 1024 / 16 + panel->x / 16 + j ];
        TVRAM_PAGE1[ ( panel->y + i ) * 1024 / 16 + panel->x / 16 + j ]
         = TVRAM_PAGE1[ ( panel->y + i - 16 ) * 1024 / 16 + panel->x / 16 + j ];
      }
    }

    // erase the top line
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < panel->width / 16; j++) {
        TVRAM_PAGE0[ ( panel->y + i ) * 1024 / 16 + j ] = 0x0000;
        TVRAM_PAGE1[ ( panel->y + i ) * 1024 / 16 + j ] = 0x0000;
      }
    }

    // decrement scroll window position here
    m->list_view_index--;

    // the new line is a sub dir? or a MDX?    
    if (m->list_view_index < mdx_list->sub_dir_count) {
      // sub dir
      int si = m->list_view_index;
      panel_put_text(panel, 4, 0 + 7, COLOR_DARK_PURPLE, FONT_REGULAR, "<DIR>");
      panel_put_text16(panel, 4 + 8*16, 0, COLOR_DARK_PURPLE, mdx_list_get_sorted_sub_dir_name(mdx_list, si, m->list_sort_order));
    } else {
      // MDX
      static unsigned char trimmed_file_name [ TRIM_FILE_NAME_LEN ];
      int mi = m->list_view_index - mdx_list->sub_dir_count;
      trim_file_name(trimmed_file_name, TRIM_FILE_NAME_LEN, mdx_list_get_sorted_file_name(mdx_list, mi, m->list_sort_order));
      panel_put_text(panel, 4, 0 + 7, COLOR_DARK_PURPLE, FONT_REGULAR, trimmed_file_name);
      panel_put_text16(panel, 4 + 8*16, 0, COLOR_PURPLE, mdx_list_get_sorted_data_title(mdx_list, mi, m->list_sort_order));
    }

    // new cursor bar
    panel_xline(panel, 4, 16 * m->list_index + 15, 760, COLOR_WHITE);
  }
}

// move cursor bar down in the list panel
void panel_mdx_list_down(PANEL* panel) {

  if (panel == NULL) return;

  MODEL* m = panel->model;
  MDX_LIST* mdx_list = m->mdx_list;

  if (mdx_list == NULL || (mdx_list->sub_dir_count + mdx_list->mdx_count) == 0) return;

  WAIT_VSYNC;
  WAIT_VBLANK;

  if (m->list_index < m->list_view_size-1) {

    if (m->list_index + 1 < mdx_list->sub_dir_count + mdx_list->mdx_count) {
      // just move cursor down
      panel_clear(panel, 4, 16 * m->list_index + 15, 760, 1, COLOR_WHITE);   // erase cursor
      panel_xline(panel, 4, 16 * (m->list_index + 1) + 15, 760, COLOR_WHITE);
      m->list_index++;
    }

  } else if (m->list_view_index +1 + m->list_view_size -1 < mdx_list->sub_dir_count + mdx_list->mdx_count) {

    // we are at the end of list, scrolling up
    panel_clear(panel, 4, 16 * m->list_index + 15, 760, 1, COLOR_WHITE);   // erase cursor
    for (int i = 0; i < 16 * (m->list_view_size - 1); i++) {
      for (int j = 0; j < panel->width / 16; j++) {
        TVRAM_PAGE0[ ( panel->y + i ) * 1024 / 16 + panel->x / 16 + j ]
         = TVRAM_PAGE0[ ( panel->y + i + 16 ) * 1024 / 16 + panel->x / 16 + j ];
        TVRAM_PAGE1[ ( panel->y + i ) * 1024 / 16 + panel->x / 16 + j ]
         = TVRAM_PAGE1[ ( panel->y + i + 16 ) * 1024 / 16 + panel->x / 16 + j ];
      }
    }

    // erase the bottom line
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < panel->width / 16; j++) {
        TVRAM_PAGE0[ ( panel->y + ( m->list_view_size - 1 ) * 16 + i ) * 1024 / 16 + j ] = 0x0000;
        TVRAM_PAGE1[ ( panel->y + ( m->list_view_size - 1 ) * 16 + i ) * 1024 / 16 + j ] = 0x0000;
      }
    }

    // increment scroll window poition here
    m->list_view_index++;

    // new line is a sub dir? or a MDX?
    if (m->list_view_index + m->list_view_size - 1 < mdx_list->sub_dir_count) {
      // sub dir
      int si = m->list_view_index + m->list_view_size -1;
      panel_put_text(panel, 4, 16 * (m->list_view_size - 1) + 7, COLOR_DARK_PURPLE, FONT_REGULAR, "<DIR>");
      panel_put_text16(panel, 4 + 8*16, 16 * (m->list_view_size - 1), COLOR_DARK_PURPLE, mdx_list_get_sorted_sub_dir_name(mdx_list, si, m->list_sort_order));
    } else {
      // MDX
      static unsigned char trimmed_file_name [ TRIM_FILE_NAME_LEN ];
      int mi = m->list_view_index + m->list_view_size -1 - mdx_list->sub_dir_count;
      trim_file_name(trimmed_file_name, TRIM_FILE_NAME_LEN, mdx_list_get_sorted_file_name(mdx_list, mi, m->list_sort_order));
      panel_put_text(panel, 4, 16 * (m->list_view_size - 1) + 7, COLOR_DARK_PURPLE, FONT_REGULAR, trimmed_file_name);
      panel_put_text16(panel, 4 + 8*16, 16 * (m->list_view_size - 1), COLOR_PURPLE, mdx_list_get_sorted_data_title(mdx_list, mi, m->list_sort_order));
    }

    // new cursor bar
    panel_xline(panel, 4, 16 * m->list_index + 15, 760, COLOR_WHITE);
  }
}

// show mdx list path
void panel_mdx_play_show_path(PANEL* panel) {
  if (panel != NULL) {

    WAIT_VBLANK;

    static unsigned char trimmed_file_name [ TRIM_FILE_NAME_LEN ];
    trim_file_name(trimmed_file_name, TRIM_FILE_NAME_LEN, panel->model->mdx_list->path_name);
    panel_clear(panel, 4, 10, 8*16, 7, COLOR_DARK_PURPLE);
    panel_put_text(panel, 4, 10, COLOR_DARK_PURPLE, FONT_BOLD, trimmed_file_name);
  }
}

// show mdx data title
void panel_mdx_play_show_title(PANEL* panel) {
  if (panel != NULL) {

    WAIT_VBLANK;

    MODEL* m = panel->model;
    panel_clear(panel, 4 + 8*16, 3, panel->width - 4 - 8*16 - 2, panel->height - 6, COLOR_WHITE);
    if ((m->mdx_list->sub_dir_count + m->mdx_list->mdx_count) > 0) {
      int mi = m->list_view_index + m->list_index - m->mdx_list->sub_dir_count;
      panel_put_text16(panel, 4 + 8*16, 3, COLOR_WHITE, mdx_list_get_sorted_data_title(m->mdx_list, mi, m->list_sort_order));
    }
  }
}