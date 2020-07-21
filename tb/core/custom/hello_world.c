#include <stdio.h>
#include <stdlib.h>

extern read_csr_vlenb();

int main(int argc, char *argv[])
{
    /* inline assembly */
    asm volatile("ecall");
    /* write something to stdout */
    printf("Hello World \n");
    printf("VLEN in Byte: %04X \n", read_csr_vlenb());
    return EXIT_SUCCESS;
}
