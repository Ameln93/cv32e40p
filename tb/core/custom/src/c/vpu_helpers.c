#include "vpu_helpers.h"

const uint32_t c_vstart  = 0x008;
const uint32_t c_vl      = 0xC20;
const uint32_t c_vtype   = 0xC21;
const uint32_t c_vlenb   = 0xC22;

inline uint32_t read_csr_vtype(void)
{
    int32_t csr_data;
    asm ("csrr %0, vlenb": "=r\t\n" (csr_data));
    return csr_data;
}
