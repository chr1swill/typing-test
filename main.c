#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include "american_english.h" 

#define clscn()                                               \
do {                                                          \
  if ((write(STDOUT_FILENO, "\033[H\033[2J",                  \
       sizeof("\033[H\033[2J"))) != sizeof("\033[H\033[2J"))  \
  {                                                           \
    perror("write: clear screen");                            \
    exit(EXIT_FAILURE);                                       \
  }                                                           \
} while(0);                                                   \

static inline 
void setrawmode(struct termios *oldt, struct termios *newt)
{
  tcgetattr(STDIN_FILENO, oldt);
  *newt = *oldt;

  newt->c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

static inline
void cleanup(struct termios *oldt)
{
  tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}
static inline
void clean_on_exit(struct termios *oldt, int status)
{
  putchar('\n');
  cleanup(oldt);
  exit(status);
}

static inline
void putchar_at_offset_white(struct termios *oldt, size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B\033[97m%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC\033[97m%c",
           offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
  }

  fflush(stdout);
}

static inline
void putchar_at_offset_red(struct termios *oldt, size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B\033[31m%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC\033[31m%c",
           offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
  }

  fflush(stdout);
}

int main()
{
  char *s, c;
  size_t sl, n, cur;
  struct termios oldt, newt;


  setrawmode(&oldt, &newt);

  clscn();

  s = "this is the string\n";
  sl = strlen(s);

  n = write(STDOUT_FILENO, s, sl);
  if (n != sl) {
    clean_on_exit(&oldt, EXIT_FAILURE);
  }

  cur = 0;

  while (1)
  {
    if (cur == sl - 1) break;

    n = read(STDIN_FILENO, &c, 1);
    if (n != 1) {
      perror("read STDIN");
      clean_on_exit(&oldt, EXIT_FAILURE);
    }

    if (c != s[cur])
    {
      putchar_at_offset_red(&oldt, cur, c);
    }
    else
    {
      putchar_at_offset_white(&oldt, cur, c);
      ++cur;
    }
  }

  clean_on_exit(&oldt, EXIT_SUCCESS);
}
