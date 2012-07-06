#ifndef __LIBS_THUMIPS_H__
#define __LIBS_THUMIPS_H__

#include <defs.h>

#define do_div(n, base) ({                                          \
            unsigned long __mod;    \
            (__mod) = ((unsigned long)n) % (base);                                \
            (n) = ((unsigned long)n) / (base);                                          \
            __mod;                                                  \
        })

#define barrier() __asm__ __volatile__ ("" ::: "memory")


static inline uint8_t inb(uint32_t port) __attribute__((always_inline));
static inline void outb(uint32_t port, uint8_t data) __attribute__((always_inline));
static inline uint32_t inw(uint32_t port) __attribute__((always_inline));
static inline void outw(uint32_t port, uint32_t data) __attribute__((always_inline));

static inline uint8_t
inb(uint32_t port) {
    uint8_t data = *((volatile uint8_t*) port);
    return data;
}

static inline void
outb(uint32_t port, uint8_t data) {
    *((volatile uint8_t*) port) = data;
}

static inline uint32_t
inw(uint32_t port) {
    uint32_t data = *((volatile uintptr_t *) port);
    return data;
}

static inline void
outw(uint32_t port, uint32_t data) {
    *((volatile uintptr_t *) port) = data;
}

/* board specification */
#define ISA_BASE        0xbfd00000
#define COM1            (ISA_BASE+0x3F8)
#define COM1_IRQ        4

#define TIMER0_IRQ       7

#endif /* !__LIBS_THUMIPS_H__ */

