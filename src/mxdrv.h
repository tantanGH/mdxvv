#include <stdint.h>
#include <stddef.h>

#define MXDRV_STATUS_PLAYING        (0x0000)
#define MXDRV_STATUS_STOPPED        (0x0001)
#define MXDRV_STATUS_PAUSED         (0xFF00)
#define MXDRV_STATUS_FADEOUT_END    (0xFF01)

// prototype declarations
uint32_t mxdrv_load_mml(uint8_t* mml_data, size_t size);
uint32_t mxdrv_load_pcm(uint8_t* pcm_data, size_t size);
uint32_t mxdrv_m_play();
uint32_t mxdrv_m_end();
uint32_t mxdrv_m_stop();
uint32_t mxdrv_m_cont();
uint8_t* mxdrv_mml_name();
uint32_t mxdrv_m_fadeout(int32_t speed);
uint16_t mxdrv_m_stat();