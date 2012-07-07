.text
.globl kernel_thread_entry
/* $a0 = arg, $a1 = func
 * see proc.c:kernel_thread
 */
kernel_thread_entry:        # void kernel_thread(void)
  nop
  nop
  addiu $sp, $sp, -16
  nop
  jal $a1
  nop
  move $a0, $v0
  la  $t0, do_exit
  jal $t0 
  nop
  /* never here */


