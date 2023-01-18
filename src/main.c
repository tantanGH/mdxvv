#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <doslib.h>
#include <process.h>
#include <direct.h>

#include "mdxvv.h"
#include "mdx.h"
#include "screen.h"
#include "panel.h"
#include "keyboard.h"
#include "opm.h"

// create model
static void create_model(MODEL* model) {

  model->voice_set = NULL;
  model->voice_index = 0;

  model->list_index = 0;
  model->list_view_size = 7;
  model->list_view_index = 0;
  model->list_sort_order = SORT_ORDER_NORMAL;

  model->message_index = 0;
  model->message_view_size = 5;

  model->original_current_drive = CURDRV();
  getcwd(model->original_current_dir, MAX_PATH_LEN);
}

// create view
static void create_view(SCREEN_HANDLE* scr, MODEL* model) {

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
  panel_put_text(p, 288, 2, COLOR_DARK_PURPLE, FONT_BOLD, "MDXVV.X");
  //panel_put_bitmap(p, 288, 0, LOGO_WIDTH, LOGO_HEIGHT, COLOR_DARK_PURPLE, mdxvv_logo_data);
  panel_put_text(p, 288 + 8*8, 2, COLOR_DARK_PURPLE, FONT_REGULAR, "v" VERSION);
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

  int x_ofs = 16;
  int y_ofs = 24;
  int y_step = 16;

  panel_xline(p, 0, p->height-1, p->width, COLOR_DARK_PURPLE);
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "PROG" );

  panel_put_text(p, x_ofs, y_ofs + y_step*0, COLOR_PURPLE, FONT_BOLD, "VOICE");
  panel_put_text(p, x_ofs, y_ofs + y_step*2, COLOR_PURPLE, FONT_BOLD, "ALG");
  panel_put_text(p, x_ofs, y_ofs + y_step*3, COLOR_PURPLE, FONT_BOLD, "FL");
  panel_put_text(p, x_ofs, y_ofs + y_step*4, COLOR_PURPLE, FONT_BOLD, "OP");

  // final wave panel
  p = screen_get_panel(scr, PANEL_FINAL_WAVE);
  p->id = PANEL_FINAL_WAVE;
  p->x = 0;
  p->y = 266;
  p->width = 160;
  p->height = 54;
  p->scr = scr;
  p->model = model;

  panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
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
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "M1 [ OP1 ]" );

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
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "C1 [ OP2 ]" );
  
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
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "M2 [ OP3 ]" );
  
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

  panel_box(p, -1, -1, p->width+1, p->height+1, COLOR_DARK_PURPLE);
//  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "OP4 ( C2 )" );
  panel_put_text_center(p, 4, COLOR_PURPLE, FONT_BOLD, "C2 [ OP4 ]" );

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

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  //panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "ENVELOPE" );
  panel_xline(p, 19, 16+35, 114, COLOR_DARK_PURPLE);
  panel_yline(p, 19, 10, 41, COLOR_DARK_PURPLE);

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
  panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
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
  panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
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
  panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
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

  panel_box(p, -1, -1, p->width + 1, p->height + 1, COLOR_DARK_PURPLE);
  panel_put_text_center(p, 4, COLOR_DARK_PURPLE, FONT_BOLD, "WAVEFORM" );
  panel_xline(p, 19, 32, 114, COLOR_DARK_PURPLE);

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

// mdx list update event helper
static MDX_LIST* update_mdx_list(SCREEN_HANDLE* scr, MODEL* m, const unsigned char* new_mdx_dir) {

  static unsigned char mes[ MAX_MES_LEN ];
  sprintf(mes, "Opening MDX directory (%s) ...", new_mdx_dir);
  panel_message_show(screen_get_panel(scr, PANEL_MESSAGE), (const unsigned char*)mes);

  MDX_LIST* new_mdx_list = mdx_list_open((const unsigned char*)new_mdx_dir);
  if (new_mdx_list == NULL) {

    panel_message_show(screen_get_panel(scr, PANEL_MESSAGE), "!!! MDX directory open error.");

  } else {

    if (m->mdx_list != NULL) {
      mdx_list_close(m->mdx_list);
    }

    m->mdx_list = new_mdx_list;
    m->list_index = 0;
    m->list_view_index = 0;
    
    // change current drive and directory
    int d0 = m->mdx_list->path_name[0];
    int d1 = d0 - ((d0 >= 'a' && d0 <= 'z') ? 'a' : 'A');      // 0:A  1:B  ...
    if (CHGDRV(d1) <= d1) {
      panel_message_show(screen_get_panel(scr, PANEL_MESSAGE), "!!! Change drive error.");
    }
    if (CHDIR(m->mdx_list->path_name) != 0) {
      panel_message_show(screen_get_panel(scr, PANEL_MESSAGE), "!!! Change directory error.");      
    }

    // refresh view
    panel_mdx_play_show_path(screen_get_panel(scr, PANEL_MDX_PLAY));
    panel_mdx_list_refresh(screen_get_panel(scr, PANEL_MDX_LIST));

    // show message
    static unsigned char mes[ MAX_MES_LEN ];
    sprintf(mes,"Loaded %d MDX files and %d sub dirs in %s.", m->mdx_list->mdx_count, m->mdx_list->sub_dir_count, m->mdx_list->path_name);
    panel_message_show(screen_get_panel(scr, PANEL_MESSAGE), mes);

  }
}

// main controller
int main(int argc, char* argv[]) {

  // exit code
  int rc = 1;

  // graphic preservation
  int preserve_graphic = 0;

  // argument check
  static unsigned char mdx_dir[ MAX_PATH_LEN ];
  strcpy(mdx_dir, ".");
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) >= 2) {
      if (argv[i][1] == 'g') {
        preserve_graphic = 1;
      } else {
        printf("error: unknown option.");
        goto exit;
      }
      continue;
    }
    strcpy(mdx_dir, argv[i]);
    break;
  }

  // supervisor mode
  B_SUPER(0);

  // create MVC model instance
  static MODEL model = { 0 };
  MODEL* m = &model;
  create_model(m);

  // create screen instance
  static SCREEN_HANDLE screen_handle = { 0 };
  SCREEN_HANDLE* scr = &screen_handle;
  screen_init(scr, preserve_graphic);
  screen_init_font(scr);

  // create MVC view
  create_view(scr, m);

  // opening message
  PANEL* panel_message = screen_get_panel(scr, PANEL_MESSAGE);
  panel_message_show(panel_message, "MDXVV.X v" VERSION);
  panel_message_show(panel_message, "Completed initialization successfully.");

  // display panel shortcuts
  PANEL* panel_mdx_play = screen_get_panel(scr, PANEL_MDX_PLAY);  
  PANEL* panel_mdx_list = screen_get_panel(scr, PANEL_MDX_LIST);

  // program and operator panels shortcuts
  PANEL* panel_con_ops = screen_get_panel(scr, PANEL_CON_OPS);
  PANEL* panel_m1_ops  = screen_get_panel(scr, PANEL_M1_OPS );
  PANEL* panel_c1_ops  = screen_get_panel(scr, PANEL_C1_OPS );
  PANEL* panel_m2_ops  = screen_get_panel(scr, PANEL_M2_OPS );
  PANEL* panel_c2_ops  = screen_get_panel(scr, PANEL_C2_OPS );
  PANEL* panel_m1_env  = screen_get_panel(scr, PANEL_M1_ENV );
  PANEL* panel_c1_env  = screen_get_panel(scr, PANEL_C1_ENV );
  PANEL* panel_m2_env  = screen_get_panel(scr, PANEL_M2_ENV );
  PANEL* panel_c2_env  = screen_get_panel(scr, PANEL_C2_ENV );

  // get the initial MDX list
  update_mdx_list(scr, m, mdx_dir);
    
  // main loop
  for (;;) {

    if (B_KEYSNS() != 0) {

      int scan_code = B_KEYINP() >> 8;

      switch (scan_code) {

        case KEY_SCAN_CODE_ESC:
        case KEY_SCAN_CODE_Q:
          // quit mdxvv
          goto quit;

        case KEY_SCAN_CODE_UP: {
          // cursor up
          panel_mdx_list_up(panel_mdx_list);
          break;
        }

        case KEY_SCAN_CODE_DOWN: {
          // cursor down
          panel_mdx_list_down(panel_mdx_list);
          break;
        }

        case KEY_SCAN_CODE_LEFT: {
          // drive change prev
          int d = m->mdx_list->path_name[0];
          int new_drv = (((d >= 'a' && d <= 'z') ? d - 'a' : d - 'A') - 1 + 26) % 26;
          while (new_drv >= 0) {
            int sns = DRVCTRL(0, 1 + new_drv);    // DRVCTRL 0:current 1:A 2:B ...
            if ((sns & 4) || !(sns & 2)) {
              //panel_message_show(panel_message, "Drive not ready or no media");
            } else {
              if (CHGDRV(new_drv) > new_drv) {    // CHGDRV 0:A 1:B 2:C ...
                static unsigned char new_mdx_dir[ MAX_PATH_LEN ];
                sprintf(new_mdx_dir, "%c:", new_drv + 'A');
                update_mdx_list(scr, m, new_mdx_dir);
                break;
              }
            }
            new_drv--;
          }
          break;
        }

        case KEY_SCAN_CODE_RIGHT: {
          // drive change next
          int d = m->mdx_list->path_name[0];
          int new_drv = (((d >= 'a' && d <= 'z') ? d - 'a' : d - 'A') + 1) % 26;
          while (new_drv <= 25) {
            int sns = DRVCTRL(0, new_drv);      // DRVCTRL 0:current 1:A 2:B ...
            if ((sns & 4) || !(sns & 2)) {
              //panel_message_show(panel_message, "Drive not ready or no media");
            } else {
              if (CHGDRV(new_drv) > new_drv) {  // CHGDRV 0:A 1:B 2:C ...                    
                static unsigned char new_mdx_dir[ MAX_PATH_LEN ];
                sprintf(new_mdx_dir, "%c:", new_drv + 'A');
                update_mdx_list(scr, m, new_mdx_dir);
                break;
              }
            }
            new_drv++;
          }
          break;
        }

        case KEY_SCAN_CODE_CR:
        case KEY_SCAN_CODE_ENTER:
        case KEY_SCAN_CODE_SPACE:
        case KEY_SCAN_CODE_BS: {

          // select and play MDX, or enter a sub directory
          if (m->mdx_list != NULL 
              && m->mdx_list->mdx_count > 0 
              && m->list_view_index + m->list_index >= m->mdx_list->sub_dir_count) {

            // selected MDX file
            static unsigned char mdx_full_path_name[ MAX_PATH_LEN ];
            static MDX mdx;

            // selected MDX index
            int mi = m->list_view_index + m->list_index - m->mdx_list->sub_dir_count;

            // full MDX path name
            strcpy(mdx_full_path_name, m->mdx_list->path_name);
            strcat(mdx_full_path_name, mdx_list_get_sorted_file_name(m->mdx_list, mi, m->list_sort_order));

            // execute external MXP.X to play the selected MDX
            static unsigned char cmd[ MDX_MAX_PATH_NAME_LEN + 32 ];
            sprintf(cmd, "%s -p %s", MXP_EXEC, mdx_full_path_name);   // for message
            panel_message_show(panel_message, cmd);
            sprintf(cmd, "%s -p %s >NUL", MXP_EXEC, mdx_full_path_name);  // for system exec
            if (system(cmd) != 0) {
              panel_message_show(panel_message, "!!! " MXP_EXEC " execution failure.");
            } else {
              panel_mdx_play_show_title(panel_mdx_play);    // show playing MDX data title
            }

            // move cursor before loading MDX (takes time)
            if (scan_code == KEY_SCAN_CODE_BS) {
              // cursor up
              panel_mdx_list_up(panel_mdx_list);
            } else if (scan_code == KEY_SCAN_CODE_SPACE) {
              // cursor down            
              panel_mdx_list_down(panel_mdx_list);
            }

            // open MDX and get voice set
            if (mdx_open(&mdx, (const unsigned char*)mdx_full_path_name) != 0) {
              panel_message_show(panel_message, "!!! MDX file open error.");
              break;
            }

            // if we have any existing voice set, discard it
            if (m->voice_set != NULL) {
              voice_set_close(m->voice_set);
              m->voice_set = NULL;
            }

            // get voice set from the opened MDX
            m->voice_set = mdx_get_voice_set(&mdx);
            m->voice_index = 0;

            // prepare adsr curves
            VOICE* v = &(m->voice_set->voices[ m->voice_index ]);
            opm_adsr(m->adsr_m1, ADSR_WIDTH, ADSR_KEY_OFF, 
                     v->attack_rate_m1, v->decay_rate1_m1, v->decay_rate2_m1, v->release_rate_m1, v->decay_level1_m1);
            opm_adsr(m->adsr_c1, ADSR_WIDTH, ADSR_KEY_OFF, 
                     v->attack_rate_c1, v->decay_rate1_c1, v->decay_rate2_c1, v->release_rate_c1, v->decay_level1_c1);
            opm_adsr(m->adsr_m2, ADSR_WIDTH, ADSR_KEY_OFF, 
                     v->attack_rate_m2, v->decay_rate1_m2, v->decay_rate2_m2, v->release_rate_m2, v->decay_level1_m2);
            opm_adsr(m->adsr_c2, ADSR_WIDTH, ADSR_KEY_OFF, 
                     v->attack_rate_c2, v->decay_rate1_c2, v->decay_rate2_c2, v->release_rate_c2, v->decay_level1_c2);

            // refresh view
            WAIT_VSYNC;
            WAIT_VBLANK;
            panel_con_refresh(panel_con_ops);
            panel_op_refresh(panel_m1_ops, 1);
            panel_op_refresh(panel_c1_ops, 2);
            panel_op_refresh(panel_m2_ops, 3);
            panel_op_refresh(panel_c2_ops, 4);

            panel_op_envelope_refresh(panel_m1_env, 1);
            panel_op_envelope_refresh(panel_c1_env, 2);
            panel_op_envelope_refresh(panel_m2_env, 3);
            panel_op_envelope_refresh(panel_c2_env, 4);

            // close MDX
            mdx_close(&mdx);

          } else if (m->mdx_list != NULL
                     && m->mdx_list->sub_dir_count > 0
                     && m->list_view_index + m->list_index < m->mdx_list->sub_dir_count
                     && (scan_code == KEY_SCAN_CODE_CR || scan_code == KEY_SCAN_CODE_ENTER)) {

            // enter sub directory
            int si = m->list_view_index + m->list_index;

            // full sub directory path name
            static unsigned char new_mdx_dir[ MAX_PATH_LEN ];
            strcpy(new_mdx_dir, m->mdx_list->path_name);
            strcat(new_mdx_dir, mdx_list_get_sorted_sub_dir_name(m->mdx_list, si, m->list_sort_order));

            update_mdx_list(scr, m, new_mdx_dir);

          } else {

            if (scan_code == KEY_SCAN_CODE_BS) {
              // cursor up
              panel_mdx_list_up(panel_mdx_list);
            } else if (scan_code == KEY_SCAN_CODE_SPACE) {
              // cursor down            
              panel_mdx_list_down(panel_mdx_list);
            }

          }

          break;
        }

        case KEY_SCAN_CODE_TAB: {

          if (m->voice_set != NULL) {
            if (B_SFTSNS() & 0x01) {
              // voice number prev
              m->voice_index = ( m->voice_index - 1 + m->voice_set->voice_count ) % m->voice_set->voice_count;
            } else {
              // voice number next
              m->voice_index = ( m->voice_index + 1 ) % m->voice_set->voice_count;
            }

            // prepare adsr curves
            VOICE* v = &(m->voice_set->voices[ m->voice_index ]);
            opm_adsr(m->adsr_m1, ADSR_WIDTH, ADSR_KEY_OFF, 
                    v->attack_rate_m1, v->decay_rate1_m1, v->decay_rate2_m1, v->release_rate_m1, v->decay_level1_m1);
            opm_adsr(m->adsr_c1, ADSR_WIDTH, ADSR_KEY_OFF, 
                    v->attack_rate_c1, v->decay_rate1_c1, v->decay_rate2_c1, v->release_rate_c1, v->decay_level1_c1);
            opm_adsr(m->adsr_m2, ADSR_WIDTH, ADSR_KEY_OFF, 
                    v->attack_rate_m2, v->decay_rate1_m2, v->decay_rate2_m2, v->release_rate_m2, v->decay_level1_m2);
            opm_adsr(m->adsr_c2, ADSR_WIDTH, ADSR_KEY_OFF, 
                    v->attack_rate_c2, v->decay_rate1_c2, v->decay_rate2_c2, v->release_rate_c2, v->decay_level1_c2);

            // wait vsync
            WAIT_VSYNC;
            WAIT_VBLANK;

            // refresh ops panels
            panel_con_refresh(panel_con_ops);
            panel_op_refresh(panel_m1_ops, 1);
            panel_op_refresh(panel_c1_ops, 2);
            panel_op_refresh(panel_m2_ops, 3);
            panel_op_refresh(panel_c2_ops, 4);

            // refresh envelope panels
            panel_op_envelope_refresh(panel_m1_env, 1);
            panel_op_envelope_refresh(panel_c1_env, 2);
            panel_op_envelope_refresh(panel_m2_env, 3);
            panel_op_envelope_refresh(panel_c2_env, 4);
          }
          break;
        }

        case KEY_SCAN_CODE_COMMA: {
          if (B_SFTSNS() & 0x01) {
            // '<' jump to list top


          }
          break;
        }

        case KEY_SCAN_CODE_PERIOD: {
          if (B_SFTSNS() & 0x01) {
            // '>' jump to list end

          } else {

            // '.' move to parent directory (if exists)
            if (strcmp(m->mdx_list->sub_dir_names + 0, "..") == 0) {

              // full sub directory path name
              static unsigned char new_mdx_dir[ MAX_PATH_LEN ];
              strcpy(new_mdx_dir, m->mdx_list->path_name);
              strcat(new_mdx_dir, "..");

              update_mdx_list(scr, m, new_mdx_dir);
            }

          }
          break;
        }

        case KEY_SCAN_CODE_YEN: {

          // '\' move to root directory
          static unsigned char new_mdx_dir[ MAX_PATH_LEN ];
          sprintf(new_mdx_dir, "%c:\\", m->mdx_list->path_name[0]);

          update_mdx_list(scr, m, new_mdx_dir);
          break;
        }

        case KEY_SCAN_CODE_R: {
          // toggle sort order
          m->list_sort_order = (m->list_sort_order == SORT_ORDER_NORMAL) ? SORT_ORDER_REVERSE : SORT_ORDER_NORMAL;
          m->list_view_index = 0;
          panel_mdx_list_refresh(panel_mdx_list);
          panel_message_show(panel_message, "Reversed sort order.");
          break;
        }

        case KEY_SCAN_CODE_S: {
          // pause playing MDX
          panel_message_show(panel_message, MXP_EXEC " -s");
          if (system(MXP_EXEC " -s >NUL") != 0) {
            panel_message_show(panel_message, "!!! " MXP_EXEC " execution failure.");
          }
          break;
        }

        case KEY_SCAN_CODE_C: {
          // resume playing MDX
          panel_message_show(panel_message, MXP_EXEC " -c");
          if (system(MXP_EXEC "-c >NUL") != 0) {
            panel_message_show(panel_message, "!!! " MXP_EXEC " execution failure.");
          }
          break;
        }

        case KEY_SCAN_CODE_E: {
          // end playing MDX
          panel_message_show(panel_message, MXP_EXEC " -e");
          if (system(MXP_EXEC "-e >NUL") != 0) {
            panel_message_show(panel_message, "!!! " MXP_EXEC " execution failure.");
          }
          break;
        }

        case KEY_SCAN_CODE_F: {
          // fade out playing MDX
          panel_message_show(panel_message, MXP_EXEC " -f");
          if (system(MXP_EXEC "-f >NUL") != 0) {
            panel_message_show(panel_message, "!!! " MXP_EXEC " execution failure.");
          }
          break;
        }

        case KEY_SCAN_CODE_M: {
          if (B_SFTSNS() & 0x01) {
            // 'M' execute external MMMDSP.r to play the selected MDX

            // selected MDX file
            static unsigned char mdx_full_path_name[ MAX_PATH_LEN ];

            // selected MDX index
            int mi = m->list_view_index + m->list_index - m->mdx_list->sub_dir_count;

            // full MDX path name
            strcpy(mdx_full_path_name, m->mdx_list->path_name);
            strcat(mdx_full_path_name, mdx_list_get_sorted_file_name(m->mdx_list, mi, m->list_sort_order));

            // execute MMDSP
            static unsigned char cmd[ MDX_MAX_PATH_NAME_LEN + 32 ];
            sprintf(cmd, "%s %s", MMDSP_EXEC, mdx_full_path_name);   // for message
            panel_message_show(panel_message, cmd);
            sprintf(cmd, "%s %s >NUL", MMDSP_EXEC, mdx_full_path_name);  // for system exec
            if (system(cmd) != 0) {
              panel_message_show(panel_message, "!!! " MMDSP_EXEC " execution failure.");
            } else {
              // recreate full view
              C_FNKMOD(3);
              C_CUROFF();
              m->list_index = 0;
              m->list_view_index = 0;
              m->message_index = 0;
              create_view(scr, m);
              panel_mdx_play_show_path(panel_mdx_play);
              panel_mdx_list_refresh(panel_mdx_list);
              //panel_mdx_play_show_title(panel_mdx_play);    // show playing MDX data title
            }
          }
          break;
        }

        case KEY_SCAN_CODE_X: {
          // export voice set
          if (m->voice_set != NULL) {
            panel_message_show(panel_message, "export!!");
          }
          break;
        }

        case KEY_SCAN_CODE_H: {
          // show help message
          panel_message_show(panel_message, "MDXVV.X version " VERSION);
          panel_message_show(panel_message, "UP/DOWN: Select MDX  CR/ENTER: Play MDX or Move Dir");
          panel_message_show(panel_message, "SPACE/BS: Play and Down/Up  S/C: Pause/Resume  </>:List Top/Bottom");
          panel_message_show(panel_message, ".: Parent Dir  \\: Root Dir  LEFT/RIGHT: Change Drive  R:Revese Sort");
          panel_message_show(panel_message, "TAB: Change Voice  X: Voice Data Export  ESC/Q: Quit MDXVV");
          break;
        }

      }
    }
  }

quit:

  // close opened voice set if exists
  if (m->voice_set != NULL) {
    voice_set_close(m->voice_set);
  }

  // close MDX list if opened
  if (m->mdx_list != NULL) {
    mdx_list_close(m->mdx_list);
  }

  // reset screen
  screen_reset(scr, preserve_graphic);

  // resume current drive and directory
  CHGDRV(m->original_current_drive);
  CHDIR(m->original_current_dir);
 
  // flush key buffer
  while (B_KEYSNS() != 0) {
    B_KEYINP();
  }
 
  rc = 0;

exit:
  return rc;
}