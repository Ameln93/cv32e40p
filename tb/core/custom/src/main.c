#include <stdio.h>
#include <stdlib.h>

#include "vpu_asm_wrapper.h"

// MAX_SIZE = 2^(STEPS*STEPSIZE
#define MAX_SIZE 4096
#define STEPS 6
#define STEPSIZE 2

//extern int __MY_SECTION_START, __MY_SECTION_END;
//const uint32_t __attribute__((section (".mySection"))) v_a[SIZE] = { 1, 2, 3, 4};

int32_t a[MAX_SIZE];
int32_t b[MAX_SIZE];
int32_t vpu_res[MAX_SIZE];
int32_t cpu_res[MAX_SIZE];
int32_t sizes[STEPS];

//-----------------------------------------------------------------------------
// Plugin Testfunction here
//-----------------------------------------------------------------------------
void vpu_test_function(size_t step) {
    vmac_e32(sizes[step], &a, &b, &vpu_res);
}

void cpu_test_function(size_t step) {
  for (int i = 0; i < sizes[step]; i++) {
    cpu_res[i] = (a[i] * b[i]) + cpu_res[i];
  }
}
//-----------------------------------------------------------------------------

void print_array(size_t size, int32_t* m) {

  for (int i = 0; i < size; i++) {
    printf("%ld ", m[i]);
  }
  printf("\n");
}

void init_glob_arrays() {
  time_t  t;

  srand((unsigned) time(&t));

  for (int i = 0; i < MAX_SIZE; i++) {

    a[i] = rand();
    b[i] = rand();
    vpu_res[i] = rand();

    cpu_res[i] = vpu_res[i];
  }
}

uint8_t compare_results(size_t step) {
  uint8_t err = 0;
  for (int i = 0; i < sizes[step]; i++) {
    if (cpu_res[i] != vpu_res[i]) {
      printf("ERROR!\n");
      printf("vpu_res: \n");
      print_array(sizes[step], vpu_res);
      printf("cpu_res: \n");
      print_array(sizes[step], cpu_res);
      err = 1;
      break;
    }
  }
  return err;
}

int main(int argc, char *argv[])
{
  int32_t err = 0;

  int32_t vpu_cyc[STEPS]   = {0};
  int32_t vpu_instr[STEPS] = {0};
  int32_t cpu_cyc[STEPS]   = {0};
  int32_t cpu_instr[STEPS] = {0};


  init_glob_arrays();

  for (int i = 0; i < STEPS; i++) {
    sizes[i] = 1;
  }

  for (size_t step = 0; step < STEPS; step++) {

    // calculate current size (power of two)
    for (int i = 0; i < ((step + 1) * STEPSIZE); i++) {
      sizes[step] *= 2;
    }

    // VPU
    reset_performance_counter();
    vpu_test_function(step);
    vpu_cyc[step]   = get_cycle_count();
    vpu_instr[step] = get_instr_count();

    // CPU only equivalent
    reset_performance_counter();
    cpu_test_function(step);
    cpu_cyc[step]    = get_cycle_count();
    cpu_instr[step]  = get_instr_count();

    err = compare_results(step);
    if (err) {
      break;
    }
  }

  printf("--------------------------------------------------\n");
  printf("|                  Results                       |\n");
  printf("|------------------------------------------------|\n");
  printf("| Array Size |       VPU       |       CPU       |\n");
  printf("|------------------------------------------------|\n");
  printf("|            | CYCLES |  INSTR | CYCLES |  INSTR |\n");
  printf("|------------------------------------------------|\n");
  for(int i = 0; i < STEPS; i++) {
    printf("|      %5ld | %6ld | %6ld | %6ld | %6ld | \n",
        sizes[i], vpu_cyc[i], vpu_instr[i], cpu_cyc[i], cpu_instr[i]
        );
  }
  printf("--------------------------------------------------\n");
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

