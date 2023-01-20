#include <stdint.h>

#define MXDRV_STATUS_PLAYING  (0x0000)
#define MXDRV_STATUS_STOPPED  (0x0001)
#define MXDRV_STATUS_PAUSED   (0xFF00)
#define MXDRV_STATUS_FADEOUT  (0xFF01)

// prototype declarations
void mxdrv_m_play();
void mxdrv_m_end();
void mxdrv_m_stop();
void mxdrv_m_cont();
uint8_t* mxdrv_mml_name();
void mxdrv_m_fadeout(int16_t speed);
uint16_t mxdrv_m_stat();