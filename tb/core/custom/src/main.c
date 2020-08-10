#include <stdio.h>
#include <stdlib.h>

#include "vpu_asm_wrapper.h"

#define SIZE 16

//extern int __MY_SECTION_START, __MY_SECTION_END;
//const uint32_t __attribute__((section (".mySection"))) v_a[SIZE] = { 1, 2, 3, 4};

int32_t a[SIZE];
int32_t b[SIZE];
int32_t vpu_res[SIZE];
int32_t cpu_res[SIZE];

void print_array(int32_t* m) {

  for (int i = 0; i < SIZE; i++) {
    printf("%ld ", m[i]);
  }
  printf("\n");
}

void init_glob_arrays() {
  for (int i = 0; i < SIZE; i++) {
    a[i] = i;
    b[i] = i + SIZE;
    vpu_res[i] = i*2;
    cpu_res[i] = i*2;
  }
}

int main(int argc, char *argv[])
{
  int32_t err = 0;
  int32_t cycles = 0;;
  int32_t instructions = 0;;

  init_glob_arrays();

  // VPU matrix multiplication
  reset_performance_counter();
  vmac_e32(SIZE, &a, &b, &vpu_res);
  cycles       = get_cycle_count();
  instructions = get_instr_count();
  printf("With VPU needed cycles:       %lu. \n", cycles);
  printf("With VPU needed instructions: %lu. \n", instructions);

  reset_performance_counter();
  for (int i = 0; i < SIZE; i++) {
    cpu_res[i] = (a[i] * b[i]) + cpu_res[i];
  }
  cycles       = get_cycle_count();
  instructions = get_instr_count();
  printf("With CPU needed cycles:       %lu. \n", cycles);
  printf("With CPU needed instructions: %lu. \n", instructions);

  for (int i = 0; i < SIZE; i++) {
    if (cpu_res[i] != vpu_res[i]) {
      err = 1;
    }
  }

  if (err == 1) {
    printf("vpu_res: \n");
    print_array(vpu_res);
    printf("cpu_res: \n");
    print_array(cpu_res);
  }

  return err;
}
  /*
     reset_performance_counter();
     uint32_t sum = 0;
     for (int row = 0; row < SIZE; row++) {
     for (int col = 0; col < SIZE; col++) {
     sum = 0;
     for(int i = 0; i < SIZE; i++) {
     sum += a[row*SIZE+i] * b[i*SIZE+col];
     }
     cpu_res[row*SIZE+col] = sum;
     }
     }
     cycles       = get_cycle_count();
     instructions = get_instr_count();
     printf("With CPU needed cycles:       %lu. \n", cycles);
     printf("With CPU needed instructions: %lu. \n", instructions);

*/

