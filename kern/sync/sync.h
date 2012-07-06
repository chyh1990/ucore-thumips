#ifndef __KERN_SYNC_SYNC_H__
#define __KERN_SYNC_SYNC_H__

#include <thumips.h>
#include <intr.h>
#include <mmu.h>
#include <asm/mipsregs.h>

static inline bool
__intr_save(void) {
  if (read_c0_status() & ST0_IE) {
    return 0;
  }
  intr_disable();
  return 1;
}

static inline void
__intr_restore(bool flag) {
    if (flag) {
        intr_enable();
    }
}

#define local_intr_save(x)      do { x = __intr_save(); } while (0)
#define local_intr_restore(x)   __intr_restore(x);

#endif /* !__KERN_SYNC_SYNC_H__ */

