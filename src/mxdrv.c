#include <stdint.h>
#include "mxdrv.h"

void mxdrv_m_play() {
	
  register uint32_t reg_d0 asm ("d0") = 0x04;    // M_PLAY

  asm volatile (
    "trap #4\n"         // trap #4
    :                   // output operand
    : "r" (reg_d0)      // input operand
    :                   // clobbered register
  );

}

void mxdrv_m_end() {
	
  register uint32_t reg_d0 asm ("d0") = 0x05;    // M_END

  asm volatile (
    "trap #4\n"         // trap #4
    :                   // output operand
    : "r" (reg_d0)      // input operand
    :                   // clobbered register
  );

}

void mxdrv_m_stop() {

	register uint32_t reg_d0 asm ("d0") = 0x06;    // M_STOP

  asm volatile (
    "trap #4\n"         // trap #4
    :                   // output operand
    : "r" (reg_d0)      // input operand
    :                   // clobbered register
  );

}

void mxdrv_m_cont() {

	register uint32_t reg_d0 asm ("d0") = 0x07;    // M_CONT

  asm volatile (
    "trap #4\n"         // trap #4
    :                   // output operand
    : "r" (reg_d0)      // input operand
    :                   // clobbered register
  );

}

uint8_t* mxdrv_mml_name() {

	register uint32_t reg_d0 asm ("d0") = 0x08;    // MMLNAME
  register uint32_t reg_d1 asm ("d1") = 0;

  asm volatile (
    "trap #4\n"         // trap #4
    : "+r"  (reg_d0)    // input & output operand
    : "r"   (reg_d1)    // input operand
    :                   // clobbered register
  );

  return (unsigned char*)reg_d0;
}

void mxdrv_m_fadeout(int16_t speed) {

	register uint32_t reg_d0 asm ("d0") = 0x0C;    // M_FADEOUT
  register uint32_t reg_d1 asm ("d1") = speed;

  asm volatile (
    "trap #4\n"         // trap #4
    :                   // output operand
    : "r"   (reg_d0),   // input operand
      "r"   (reg_d1)    // input operand
    :                   // clobbered register
  );

}

uint16_t mxdrv_m_stat() {

	register uint32_t reg_d0 asm ("d0") = 0x12;    // M_STAT

  asm volatile (
    "trap #4\n"         // trap #4
    : "+r"  (reg_d0)    // input & output operand
    :                   // input operand
    :                   // clobbered register
  );

  return reg_d0 & 0xffff;
}