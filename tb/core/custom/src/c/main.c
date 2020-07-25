#include <stdio.h>
#include <stdlib.h>

#include "vpu_asm_wrapper.h"

#define SIZE 8

extern int __MY_SECTION_START, __MY_SECTION_END;

uint32_t __attribute__((section (".mySection"))) v_a[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8};

//const uint32_t __attribute__((section (".mySection"))) v_b[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//const uint32_t __attribute__((section (".mySection"))) golden[SIZE] = { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100};
uint8_t v_res[SIZE] = {0};

uint32_t main(int argc, char *argv[])
{
  uint8_t err = 0;;
  uint32_t vl = 0;;
  uint8_t *start = NULL;
  uint8_t *end   = NULL;


  printf("Try to set VL to 10 and sew to 32. \n");
  vl = setvli_e32(10);
  printf("Returned vl = %d.\n", vl);

  printf("VTYPE:        %04X \n", read_csr_vtype()  );
  printf("VL:           %04X \n", read_csr_vl()     );
  printf("VLEN in Byte: %04X \n", read_csr_vlenb()  );

  start = (uint8_t*) &__MY_SECTION_START;
  end   = (uint8_t*) &__MY_SECTION_END;

  printf("My section start address: %p. \n", start);
  printf("My section end address:   %p. \n", end);

  printf("Load Address: %p. \n", start);
  vpu_load(start);
  printf("Store Address: %p. \n", (start + 16));
  vpu_store(start+16);

  for (int i = 0; i < 4; i++) {
    if (  v_a[i] != v_a[i+4] ) {
      printf("A: %d. \n", v_a[i  ]);
      printf("B: %d. \n", v_a[i+4]);
      err = 1;
    }
  }

  return err;
}
