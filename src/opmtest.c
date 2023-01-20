#include <stdio.h>
#include <stdint.h>
#include "opm.h"

void main() {

  #define ENV_LEN (112)

  static float adsr[ENV_LEN];
  opm_adsr(adsr, ENV_LEN, 80, 25, 12, 5, 6, 4);

  for (int32_t i = 0; i < ENV_LEN; i++) {
    printf("adsr[%d]=%f\n",i,adsr[i]);
  }
}