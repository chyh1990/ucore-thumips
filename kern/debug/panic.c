#include <defs.h>
#include <stdio.h>
#include <intr.h>
#include <monitor.h>

static bool is_panic = 0;

/* *
 * __panic - __panic is called on unresolvable fatal errors. it prints
 * "panic: 'message'", and then enters the kernel monitor.
 * */
void
__panic(const char *file, int line, const char *fmt) {
    if (is_panic) {
        goto panic_dead;
    }
    is_panic = 1;

    // print the 'message'
    cprintf("kernel panic at ");
    cprintf(file);
    cprintf(":");
    printbase10(line);
    cprintf("\n      ");
    cprintf(fmt);
    cprintf("\n");

panic_dead:
    intr_disable();
    while (1) {
        monitor(NULL);
    }
}

/* __warn - like panic, but don't */
void
__warn(const char *file, int line, const char *fmt) {
    cprintf("kernel warning at ");
    cprintf("\n");
}

bool
is_kernel_panic(void) {
    return is_panic;
}

