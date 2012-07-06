#ifndef __LIBS_STDIO_H__
#define __LIBS_STDIO_H__

#include <defs.h>
#include <stdarg.h>

/* kern/libs/stdio.c */
int cprintf(const char *str);
void cputchar(int c);
int cputs(const char *str);
int getchar(void);

void printhex(unsigned int x);
void printbase10(int x);

/* kern/libs/readline.c */
char *readline(const char *prompt);

#define PRINT_HEX(str,x) {cprintf(str);printhex((unsigned int)x);cprintf("\n");}

#endif /* !__LIBS_STDIO_H__ */


