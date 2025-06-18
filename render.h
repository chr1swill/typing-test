#ifndef _RENDER_H
#define _RENDER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

#define clscn()                                               \
do {                                                          \
  if ((write(STDOUT_FILENO, "\033[H\033[2J",                  \
       sizeof("\033[H\033[2J"))) != sizeof("\033[H\033[2J"))  \
  {                                                           \
    perror("write: clear screen");                            \
    exit(EXIT_FAILURE);                                       \
  }                                                           \
} while(0);                                                   \

void setrawmode(struct termios *oldt,
                struct termios *newt);

void cleanup(struct termios *oldt);

void clean_on_exit(struct termios *oldt, int status);

void putchar_at_offset_white(struct termios *oldt,
                             size_t offset, char c);

void putchar_at_offset_red(struct termios *oldt,
                            size_t offset, char c);
#endif // _RENDER_H
