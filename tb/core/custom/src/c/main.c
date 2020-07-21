#include <stdio.h>
#include <stdlib.h>
#include "vpu_helpers.h"

int main(int argc, char *argv[])
{
  printf("VLEN in Byte: %04X \n", read_csr_vtype());
  return EXIT_SUCCESS;
}
