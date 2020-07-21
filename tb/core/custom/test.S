.section .text
.align 2
.globl read_csr_vlenb

read_csr_vlenb:

  addi sp, sp, -16  # setup stack frame
  sw ra, 12(sp)     # store return address

  csrr a0, vlenb    # read csr vlenb to the return value

  lw ra, 12(sp)     # load return address
  addi sp, sp, 16   # free strackframe
  ret               # return

