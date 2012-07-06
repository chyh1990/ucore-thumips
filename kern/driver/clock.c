#include <thumips.h>
#include <trap.h>
#include <stdio.h>
#include <picirq.h>
#include <asm/mipsregs.h>

volatile size_t ticks;

#define TIMER0_INTERVAL  1000000 

static void reload_timer()
{
  uint32_t counter = read_c0_count();
  counter += TIMER0_INTERVAL;
  write_c0_compare(counter);
}

int clock_int_handler(void * data)
{
  ticks++;
  if(ticks % 100 == 0)
    cons_putc('A');
  
  reload_timer(); 
  return 0;
}

void
clock_init(void) {
  reload_timer(); 
  pic_enable(TIMER0_IRQ);
  cprintf("++TODO setup timer interrupts\n");
}

