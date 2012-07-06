#include <defs.h>
#include <asm/mipsregs.h>
#include <clock.h>
#include <trap.h>
#include <thumips.h>
#include <stdio.h>
#include <mmu.h>
#include <memlayout.h>
#include <assert.h>
#include <console.h>
#include <kdebug.h>

#define TICK_NUM 100

#define GET_CAUSE_EXCODE(x)   ( ((x) & CAUSEF_EXCCODE) >> CAUSEB_EXCCODE)

static void print_ticks() {
    PRINT_HEX("%d ticks\n",TICK_NUM);
}



static const char *
trapname(int trapno) {
    static const char * const excnames[] = {
        "Interrupt",
        "TLB Modify",
        "TLB miss on load",
        "TLB miss on store",
        "Address error on load",
        "Address error on store",
        "Bus error on instruction fetch",
        "Bus error on data load or store",
        "Syscall",
        "Breakpoint",
        "Reserved (illegal) instruction",
        "Coprocessor unusable",
        "Arithmetic overflow",
    };
    if(trapno <= 12)
      return excnames[trapno];
    else
      return "Unknown";
}

bool
trap_in_kernel(struct trapframe *tf) {
  return !(tf->tf_status & ST0_KUP);
}


void print_regs(struct pushregs *regs)
{
  int i;
  for (i = 0; i < 30; i++) {
    cprintf(" $");
    printbase10(i+1);
    cprintf("\t: ");
    printhex(regs->reg_r[i]);
    cputchar('\n');
  }
}

void
print_trapframe(struct trapframe *tf) {
    PRINT_HEX("trapframe at ", tf);
    print_regs(&tf->tf_regs);
    PRINT_HEX(" $ra\t: ", tf->tf_ra);
    PRINT_HEX(" BadVA\t: ", tf->tf_vaddr);
    PRINT_HEX(" Status\t: ", tf->tf_status);
    PRINT_HEX(" Cause\t: ", tf->tf_cause);
    PRINT_HEX(" EPC\t: ", tf->tf_epc);
    if (!trap_in_kernel(tf)) {
      cprintf("Trap in usermode: ");
    }else{
      cprintf("Trap in kernel: ");
    }
    cprintf(trapname(GET_CAUSE_EXCODE(tf->tf_cause)));
    cputchar('\n');
}

static void interrupt_handler(struct trapframe *tf)
{
  extern clock_int_handler(void*);
  extern serial_int_handler(void*);
  int i;
  for(i=0;i<8;i++){
    if(tf->tf_cause & (1<<(CAUSEB_IP+i))){
      switch(i){
        case TIMER0_IRQ:
          clock_int_handler(NULL);
          break;
        case COM1_IRQ:
          serial_int_handler(NULL);
          break;
        default:
          print_trapframe(tf);
          panic("Unknown interrupt!");
      }
    }
  }

}

static void
trap_dispatch(struct trapframe *tf) {
  int code = GET_CAUSE_EXCODE(tf->tf_cause);
  switch(code){
    case EX_IRQ:
      interrupt_handler(tf);
      break;
    case EX_TLBL:
    case EX_TLBS:
      print_trapframe(tf);
      while(1);
      break;
    case EX_RI:
      print_trapframe(tf);
      while(1);
      break;
    case EX_SYS:
      print_trapframe(tf);
      tf->tf_epc += 4;
      break;
    default:
      print_trapframe(tf);
      while(1);
  }

}


/*
 * General trap (exception) handling function for mips.
 * This is called by the assembly-language exception handler once
 * the trapframe has been set up.
 */
void
mips_trap(struct trapframe *tf)
{
  trap_dispatch(tf);
}

