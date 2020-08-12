#include "vpu_asm_wrapper.h"

const uint32_t c_vstart  = 0x008;
const uint32_t c_vl      = 0xC20;
const uint32_t c_vtype   = 0xC21;
const uint32_t c_vlenb   = 0xC22;


// vector-vector add routine of 32-bit integers
// { for (size_t i=0; i<n; i++) { z[i]=x[i]+y[i]; } }
// a0 = n, a1 = x, a2 = y, a3 = z
inline void vadd_e32(size_t n, const int32_t *a, const int32_t *b, int32_t *dest){
  asm (
      "asm_vadd_e32:                  \n\t"// Set loop label
      "vsetvli   t0,    %[n],   e32   \n\t"// Set vector length based on 32-bit vectors
      "vle32.v   v0,    (%[a])        \n\t"// Get first vector v0
      "sub       %[n],  %[n],   t0    \n\t"// Decrement number done
      "slli      t0,    t0,     2     \n\t"// Multiply number done by 4 bytes
      "add       %[a],  %[a],   t0    \n\t"// Bump pointer a
      "vle32.v   v1,    (%[b])        \n\t"// Get second vector v1
      "add       %[b],  %[b],   t0    \n\t"// Bump pointer b
      "vadd.vv   v2,    v0,     v1    \n\t"// Sum vectors
      "vse32.v   v2,    (%[d])        \n\t"// Store result
      "add       %[d],  %[d],   t0    \n\t"// Bump pointer d
      "bnez      %[n],  asm_vadd_e32  \n\t"// Loop back
      : // no outputs
      : [n] "r" (n), [a] "r" (a), [b] "r" (b), [d] "r" (dest)
      );
}
// vector-vector add routine of 16-bit integers
// { for (size_t i=0; i<n; i++) { z[i]=x[i]+y[i]; } }
// a0 = n, a1 = x, a2 = y, a3 = z
inline void vadd_e16(size_t n, const int16_t *a, const int16_t *b, int16_t *dest){
  asm (
      "asm_vadd_e16:                  \n\t"// Set loop label
      "vsetvli   t0,    %[n],   e16   \n\t"// Set vector length based on 32-bit vectors
      "vle32.v   v0,    (%[a])        \n\t"// Get first vector v0
      "sub       %[n],  %[n],   t0    \n\t"// Decrement number done
      "slli      t0,    t0,     1     \n\t"// Multiply number done by 4 bytes
      "add       %[a],  %[a],   t0    \n\t"// Bump pointer a
      "vle32.v   v1,    (%[b])        \n\t"// Get second vector v1
      "add       %[b],  %[b],   t0    \n\t"// Bump pointer b
      "vadd.vv   v2,    v0,     v1    \n\t"// Sum vectors
      "vse32.v   v2,    (%[d])        \n\t"// Store result
      "add       %[d],  %[d],   t0    \n\t"// Bump pointer d
      "bnez      %[n],  asm_vadd_e16  \n\t"// Loop back
      : // no outputs
      : [n] "r" (n), [a] "r" (a), [b] "r" (b), [d] "r" (dest)
      );
}
// vector-vector add routine of 8-bit integers
// { for (size_t i=0; i<n; i++) { z[i]=x[i]+y[i]; } }
// a0 = n, a1 = x, a2 = y, a3 = z
inline void vadd_e8(size_t n, const int16_t *a, const int16_t *b, int16_t *dest){
  asm (
      "asm_vadd_e8:                   \n\t"// Set loop label
      "vsetvli   t0,    %[n],   e8    \n\t"// Set vector length based on 32-bit vectors
      "vle32.v   v0,    (%[a])        \n\t"// Get first vector v0
      "sub       %[n],  %[n],   t0    \n\t"// Decrement number done
      "add       %[a],  %[a],   t0    \n\t"// Bump pointer a
      "vle32.v   v1,    (%[b])        \n\t"// Get second vector v1
      "add       %[b],  %[b],   t0    \n\t"// Bump pointer b
      "vadd.vv   v2,    v0,     v1    \n\t"// Sum vectors
      "vse32.v   v2,    (%[d])        \n\t"// Store result
      "add       %[d],  %[d],   t0    \n\t"// Bump pointer d
      "bnez      %[n],  asm_vadd_e8   \n\t"// Loop back
      : // no outputs
      : [n] "r" (n), [a] "r" (a), [b] "r" (b), [d] "r" (dest)
      );
}


// vector-vector mac routine of 32-bit integers
// { for (size_t i=0; i<n; i++) { z[i]=x[i]+y[i]; } }
inline void vmac_e32(size_t n, const int32_t *f1, const int32_t *f2, int32_t *acc){
  asm (
      "asm_vmac_e32:                  \n\t"// Set loop label
      "vsetvli   t0,    %[n],   e32   \n\t"// Set vector length based on 32-bit vectors
      "vle32.v   v0,    (%[a])        \n\t"// Get first vector v0
      "sub       %[n],  %[n],   t0    \n\t"// Decrement number done
      "slli      t0,    t0,     2     \n\t"// Multiply number done by 4 bytes
      "add       %[a],  %[a],   t0    \n\t"// Bump pointer a
      "vle32.v   v1,    (%[b])        \n\t"// Get second vector v1
      "vle32.v   v2,    (%[c])        \n\t"// Get third vector v2
      "add       %[b],  %[b],   t0    \n\t"// Bump pointer b
      "vmacc.vv  v2,    v0,     v1    \n\t"// Multiply add
      "vse32.v   v2,    (%[c])        \n\t"// Store result
      "add       %[c],  %[c],   t0    \n\t"// Bump pointer c
      "bnez      %[n],  asm_vmac_e32  \n\t"// Loop back
      : // no outputs
      : [n] "r" (n), [a] "r" (f1), [b] "r" (f2), [c] "r" (acc)
      );
}
// vector-vector mac routine of 16-bit integers
// { for (size_t i=0; i<n; i++) { z[i]=x[i]+y[i]; } }
inline void vmac_e16(size_t n, const int16_t *f1, const int16_t *f2, int16_t *acc){
  asm (
      "asm_vmac_e16:                  \n\t"// Set loop label
      "vsetvli   t0,    %[n],   e16   \n\t"// Set vector length based on 32-bit vectors
      "vle32.v   v0,    (%[a])        \n\t"// Get first vector v0
      "sub       %[n],  %[n],   t0    \n\t"// Decrement number done
      "slli      t0,    t0,     1     \n\t"// Multiply number done by 2 bytes
      "add       %[a],  %[a],   t0    \n\t"// Bump pointer a
      "vle32.v   v1,    (%[b])        \n\t"// Get second vector v1
      "vle32.v   v2,    (%[c])        \n\t"// Get third vector v2
      "add       %[b],  %[b],   t0    \n\t"// Bump pointer b
      "vmacc.vv  v2,    v0,     v1    \n\t"// Multiply add
      "vse32.v   v2,    (%[c])        \n\t"// Store result
      "add       %[c],  %[c],   t0    \n\t"// Bump pointer c
      "bnez      %[n],  asm_vmac_e16  \n\t"// Loop back
      : // no outputs
      : [n] "r" (n), [a] "r" (f1), [b] "r" (f2), [c] "r" (acc)
      );
}
// vector-vector mac routine of 16-bit integers
// { for (size_t i=0; i<n; i++) { z[i]=x[i]+y[i]; } }
inline void vmac_e8(size_t n, const int16_t *f1, const int16_t *f2, int16_t *acc){
  asm (
      "asm_vmac_e8:                   \n\t"// Set loop label
      "vsetvli   t0,    %[n],   e16   \n\t"// Set vector length based on 32-bit vectors
      "vle32.v   v0,    (%[a])        \n\t"// Get first vector v0
      "sub       %[n],  %[n],   t0    \n\t"// Decrement number done
      "add       %[a],  %[a],   t0    \n\t"// Bump pointer a
      "vle32.v   v1,    (%[b])        \n\t"// Get second vector v1
      "vle32.v   v2,    (%[c])        \n\t"// Get third vector v2
      "add       %[b],  %[b],   t0    \n\t"// Bump pointer b
      "vmacc.vv  v2,    v0,     v1    \n\t"// Multiply add
      "vse32.v   v2,    (%[c])        \n\t"// Store result
      "add       %[c],  %[c],   t0    \n\t"// Bump pointer c
      "bnez      %[n],  asm_vmac_e8   \n\t"// Loop back
      : // no outputs
      : [n] "r" (n), [a] "r" (f1), [b] "r" (f2), [c] "r" (acc)
      );
}
inline void reset_performance_counter(void)
{
  asm ("csrw %0, %1 \n\t" :: "n" (0xcc0), "r" (3)); // enable saturation and counters in general
  asm ("csrw %0, %1 \n\t" :: "n" (0xcc1), "r" (1)); // enable cycle and instruction counter
  asm ("csrw %0, %1 \n\t" :: "n" (0x780), "r" (0)); // set cycle counter to zero
  asm ("csrw %0, %1 \n\t" :: "n" (0x781), "r" (0)); // set instruction counter to zero
}

inline uint32_t get_cycle_count(void)
{
  uint32_t cycles = 0;
  asm ("csrr %0, %1 \n\t" : "=r" (cycles): "n" (0x780));
  return cycles;
}
inline uint32_t get_instr_count(void)
{
  uint32_t instr = 0;
  asm ("csrr %0, %1 \n\t" : "=r" (instr): "n" (0x781));
  return instr;
}

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
