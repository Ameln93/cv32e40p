#include <stdio.h>
#include <stdlib.h>

#include "vpu_asm_wrapper.h"

const uint32_t CRAND_VL = 10;

uint32_t main(int argc, char *argv[])
{
  uint32_t vl = CRAND_VL;

  printf("Try to set VL to %d and sew to 32. \n", CRAND_VL);
  vl = setvli_e32(10);
  printf("Returned vl = %d.\n", vl);

  printf("VTYPE:        %04X \n", read_csr_vtype()  );
  printf("VL:           %04X \n", read_csr_vl()     );
  printf("VLEN in Byte: %04X \n", read_csr_vlenb()  );

  return EXIT_SUCCESS;
}
