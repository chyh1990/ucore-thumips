#include <stdio.h>
#include <ulib.h>

int
main(void) {
    sys_putc('&');
    cprintf("Hello world!!.\n");
    //cprintf("I am process %d.\n", getpid());
    //cprintf("hello pass.\n");
    while(1);
    return 0;
}

