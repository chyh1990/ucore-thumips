
#include <asm/mipsregs.h>

.global kernel_entry
.extern kern_init

.extern edata
.extern end

.set noreorder
.set STACKSIZE, 0x1000

.section .startup

kernel_entry:
  b reset
  nop

	.align 4
reset:
  /* from u-boot */
	/* Clear watch registers */
	mtc0	$zero, CP0_WATCHLO
	mtc0	$zero, CP0_WATCHHI

	/* WP(Watch Pending), SW0/1 should be cleared */
	mtc0	$zero, CP0_CAUSE
  /* clear SR(ERL), which is 1 after reset */
	mtc0	$zero, CP0_STATUS

  jal 1f
  nop
  .word _gp
1:
  lw $gp, 0($ra)
  la $sp, stack+STACKSIZE-32
#setup ram exception
  la   $t0, __exception_vector
  mtc0 $t0, $15, 1 

  mfc0 $t0, CP0_STATUS
  li   $t1, ~ST0_BEV
  and  $t0, $t0, $t1
  mtc0 $t0, CP0_STATUS

#zero bss
  la $a0, edata
  la $a1, end
  
2:
  sw $zero, 0($a0)
  addiu $a0, $a0, 4
  slt $a3, $a0, $a1
  bgtz $a3, 2b
  nop 

  la $t9, kern_init 
  jal $t9
  nop
  bgez $zero, .
  nop


# bfc00280: R4000 xtlbmiss vector
romExcHandle_280:
  b romExcHandle_280
  nop

# bfc00300: R4000 cache vector
romExcHandle_300:
  b romExcHandle_300
  nop


.section .bss
stack:
  .space STACKSIZE


