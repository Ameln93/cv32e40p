#ifndef CUSTOM_INSN_H
#define CUSTOM_INSN_H

#include "stdint.h"

extern const uint32_t c_vstart;
extern const uint32_t c_vl;
extern const uint32_t c_vtype;
extern const uint32_t c_vlenb;

uint32_t read_csr_vtype(void);

#endif //CUSTOM_INSN_H
