#ifndef CUSTOM_INSN_H
#define CUSTOM_INSN_H

#include "stddef.h"
#include "stdint.h"

extern const uint32_t c_vstart;
extern const uint32_t c_vl;
extern const uint32_t c_vtype;
extern const uint32_t c_vlenb;

typedef enum {SEW_8, SEW_16, SEW_32} e_sew;

void     vadd_e32(size_t, const int32_t*, const int32_t*, int32_t*);
void     reset_cycle_count(void);
uint32_t get_cycle_count(void);

uint32_t read_vpu_csr(uint32_t);
uint32_t setvli(e_sew, uint32_t);

uint32_t setvli_e8(uint32_t);
uint32_t setvli_e16(uint32_t);
uint32_t setvli_e32(uint32_t);

uint32_t read_csr_vtype(void);
uint32_t read_csr_vlenb(void);
uint32_t read_csr_vl(void);
uint32_t read_csr_vstart(void);

#endif //CUSTOM_INSN_H
