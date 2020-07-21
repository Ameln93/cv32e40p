#include "vpu_asm_wrapper.h"

const uint32_t c_vstart  = 0x008;
const uint32_t c_vl      = 0xC20;
const uint32_t c_vtype   = 0xC21;
const uint32_t c_vlenb   = 0xC22;

uint32_t setvli(e_sew sew, uint32_t avl)
{
  int32_t vl = 0;

  switch (sew) {
    case SEW_8:
      vl = setvli_e8(avl);
      break;
    case SEW_16:
      vl = setvli_e16(avl);
      break;
    case SEW_32:
      vl = setvli_e32(avl);
      break;
    default:
      break;
  }
  return vl;
}

uint32_t read_vpu_csr(uint32_t csr_addr)
{
  int32_t csr_data = 0;

  switch (csr_addr) {

    case c_vstart:
      csr_data = read_csr_vstart();
      break;

    case c_vl:
      csr_data = read_csr_vl();
      break;

    case c_vtype:
      csr_data = read_csr_vtype();
      break;

    case c_vlenb:
      csr_data = read_csr_vlenb();
      break;

    default:
      csr_data = 0;
      break;
  }
  return csr_data;
}

inline uint32_t setvli_e8(uint32_t avl)
{
  int32_t vl = 0;
  asm ("vsetvli %0, %1, e8": "=r" (vl) : "r" (avl) );
  return vl;
}

inline uint32_t setvli_e16(uint32_t avl)
{
  int32_t vl = 0;
  asm ("vsetvli %0, %1, e16": "=r" (vl) : "r" (avl) );
  return vl;
}

inline uint32_t setvli_e32(uint32_t avl)
{
  int32_t vl = 0;
  asm ("vsetvli %0, %1, e32": "=r" (vl) : "r" (avl) );
  return vl;
}

inline uint32_t read_csr_vstart(void)
{
    int32_t csr_data;
    asm ("csrr %0, vstart": "=r" (csr_data) );
    return csr_data;
}

inline uint32_t read_csr_vl(void)
{
    int32_t csr_data = 0;
    asm ("csrr %0, vl": "=r" (csr_data) );
    return csr_data;
}

inline uint32_t read_csr_vtype(void)
{
    int32_t csr_data = 0;
    asm ("csrr %0, vtype": "=r" (csr_data) );
    return csr_data;
}

inline uint32_t read_csr_vlenb(void)
{
    int32_t csr_data = 0;
    asm ("csrr %0, vlenb": "=r" (csr_data) );
    return csr_data;
}
