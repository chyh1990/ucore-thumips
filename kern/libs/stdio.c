#include <defs.h>
#include <stdio.h>
#include <console.h>

/* HIGH level console I/O */

/* *
 * cputch - writes a single character @c to stdout, and it will
 * increace the value of counter pointed by @cnt.
 * */
static void
cputch(int c, int *cnt) {
    cons_putc(c);
    (*cnt) ++;
}

/* *
 * cprintf - formats a string and writes it to stdout
 *
 * The return value is the number of characters which would be
 * written to stdout.
 * */
int
cprintf(const char *str) {
    int cnt = 0;
    while(*str){
      cputchar(*str);
      cnt++;
      str++;
    }
    //va_start(ap, fmt);
    //cnt = vcprintf(fmt, ap);
    //va_end(ap);
    return cnt;
}


static const char* hexdigits = "0123456789ABCDEF";
void printhex(unsigned int x){
  char tmp[9];
  int i=0;
  tmp[8] = 0;
  for(i=7;i>=0;i--){
    tmp[i] = hexdigits[x & 0xf];
    x = x >> 4;
  }
  cprintf(tmp);
}

static inline unsigned int __mulu10(unsigned int n)
{
  return (n<<3)+(n<<1);
}

static inline unsigned int divu10(unsigned int n) {
  unsigned int q, r;
  q = (n >> 1) + (n >> 2);
  q = q + (q >> 4);
  q = q + (q >> 8);
  q = q + (q >> 16);
  q = q >> 3;
  r = n - __mulu10(q);
  return q + ((r + 6) >> 4);
}

void printbase10(int x){
  unsigned int t;
  int i = 0;
  char buf[16];
  if(x<0)
    cputchar('-');
  x = (x<0)?-x:x;
  while(x >= 10){
    t = divu10(x); 
    buf[i++] = ('0'+(x-__mulu10(t)));
    x = t;
  }
  buf[i] = ('0'+x);
  for(;i>=0;i--){
    cputchar(buf[i]);
  }
}

/* cputchar - writes a single character to stdout */
void
cputchar(int c) {
  cons_putc(c);
}

/* *
 * cputs- writes the string pointed by @str to stdout and
 * appends a newline character.
 * */
int
cputs(const char *str) {
    int cnt = 0;
    char c;
    while ((c = *str ++) != '\0') {
        cputch(c, &cnt);
    }
    cputch('\n', &cnt);
    return cnt;
}

/* getchar - reads a single non-zero character from stdin */
int
getchar(void) {
    int c;
    while ((c = cons_getc()) == 0)
        /* do nothing */;
    return c;
}

