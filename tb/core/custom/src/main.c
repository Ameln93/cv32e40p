#include <stdio.h>
#include <stdlib.h>

#include "vpu_asm_wrapper.h"

#define SIZE 8

//extern int __MY_SECTION_START, __MY_SECTION_END;
//const uint32_t __attribute__((section (".mySection"))) v_a[SIZE] = { 1, 2, 3, 4};

const int32_t v_a[SIZE]     = { 1, 2, 3, 4,  5,  6,  7,  8};
const int32_t v_b[SIZE]     = { 1, 2, 3, 4,  5,  6,  7,  8};
const int32_t golden[SIZE]  = { 2, 4, 6, 8, 10, 12, 14,  0};
uint32_t v_res[SIZE] = {0};

uint32_t main(int argc, char *argv[])
{
  int8_t err = 0;;
  int32_t cycles = 0;;
  /*
  int32_t vl = 0;;
  printf("Try to set VL to 10 and sew to 32. \n");
  vl = setvli_e32(10);
  printf("Returned vl = %l.\n", vl);
  */

  reset_cycle_count();
  vadd_e32(SIZE-1, v_a, v_b, v_res);
  cycles = get_cycle_count();

  for (int i = 0; i < SIZE; i++) {
    if (v_res[i] != golden[i] ) {
      printf("RES: %l. \n", v_res[i]  );
      printf("EXP: %l. \n", golden[i] );
      err = 1;
    }
  }

  if (err == 0) {
    printf("Needed cycles %lu. \n", cycles);
  }

  return err;
}
