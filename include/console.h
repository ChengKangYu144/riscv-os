#ifndef CONSOLE_H
#define CONSOLE_H

void uart_init(void);
char uart_getc(void);
void uart_putc(char c);

void console_putc(char c);
void console_puts(const char *s);
void clear_screen(void);

int printf(const char *fmt, ...);

#endif
