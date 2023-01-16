#ifndef __H_SCREEN__
#define __H_SCREEN__

#include <iocslib.h>
#include "panel.h"

//#define GVRAM             ((volatile unsigned short*)0xC00000)
#define TVRAM_PAGE0       ((volatile unsigned short*)0xE00000)
#define TVRAM_PAGE1       ((volatile unsigned short*)0xE20000)

#define REG_GPIP          ((volatile unsigned char*)0xE88001)     // generic I/O port (Inside X68000 p81)

#define WAIT_VSYNC        while(!(REG_GPIP[0] & 0x10))
#define WAIT_VBLANK       while(REG_GPIP[0] & 0x10)

#define PALETTE_COLOR0    (0x0000)
#define PALETTE_COLOR1    (0xc631)    // plane1: white
#define PALETTE_COLOR2    (0x8c7f)    // plane2: purple
#define PALETTE_COLOR3    (0x6335)    // plane1+2: dark purple

#define COLOR_BLACK        (0)
#define COLOR_WHITE        (1)
#define COLOR_PURPLE       (2)
#define COLOR_DARK_PURPLE  (3)

#define FONT_ADDR_8x8     ((unsigned char*)0xF3A000)
#define FONT_BYTES_8x8    (8)

#define FONT_REGULAR      (0)
#define FONT_BOLD         (1)

// screen class
typedef struct {

  // panels
  PANEL panel_all;
  PANEL panel_menu;

  PANEL panel_con_ops;
  PANEL panel_final_wave;

  PANEL panel_m1_ops;
  PANEL panel_c1_ops;
  PANEL panel_m2_ops;
  PANEL panel_c2_ops;

  PANEL panel_m1_env;
  PANEL panel_c1_env;
  PANEL panel_m2_env;
  PANEL panel_c2_env;

  PANEL panel_m1_wave;
  PANEL panel_c1_wave;
  PANEL panel_m2_wave;
  PANEL panel_c2_wave;

  PANEL panel_mdx_play;
  PANEL panel_mdx_list;
  
  PANEL panel_message;
  
  // text palette colors
  unsigned short tpalette[ 4 ];

  // original graphic palettes
  unsigned short original_gpalette[ 16 ];

  // original text palettes
  unsigned short original_tpalette[ 4 ];

  // original function key display mode
  int original_fnk_mode;

  // 8x8 normal font data
  struct FNTBUF font_data_8x8[ 256 ];

  // 8x8 bold font data
  struct FNTBUF font_data_8x8_bold[ 256 ];

} SCREEN_HANDLE;

// prototype declarations for SCREEN_HANDLE
void screen_init(SCREEN_HANDLE* scr, int mode);
void screen_reset(SCREEN_HANDLE* scr, int mode);
void screen_init_font(SCREEN_HANDLE* scr);
PANEL* screen_get_panel(SCREEN_HANDLE* scr, int panel_id);


#endif