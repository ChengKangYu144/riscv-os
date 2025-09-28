#include "../include/console.h"
#include <stdarg.h>

void console_putc(char c) {
    if (c == '\n') console_putc('\r');
    uart_putc(c);
}

void console_puts(const char *s) {
    if (!s) s = "(null)";
    while (*s) console_putc(*s++);
}

static void print_number(int num, int base, int sign) {
    char buf[32];
    int i = 0;
    unsigned int n;

    if (sign && num < 0) {
        if (num == -2147483648) {
            n = 2147483648U;
        } else {
            n = -num;
        }
    } else {
        n = num;
    }

    do {
        int digit = n % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        n /= base;
    } while (n);

    if (sign && num < 0) buf[i++] = '-';

    while (i--) console_putc(buf[i]);
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            console_putc(*p);
            continue;
        }
        p++;
        switch (*p) {
            case 'd': print_number(va_arg(ap,int),10,1); break;
            case 'x': print_number(va_arg(ap,int),16,0); break;
            case 'c': console_putc((char)va_arg(ap,int)); break;
            case 's': console_puts(va_arg(ap,const char*)); break;
            case '%': console_putc('%'); break;
            default:
                console_putc('%');
                console_putc(*p);
                break;
        }
    }

    va_end(ap);
    return 0;
}

void clear_screen(void) {
    console_puts("\033[2J\033[H");
}
