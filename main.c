#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

#define clscn()                                               \
do {                                                          \
  if ((write(STDOUT_FILENO, "\033[H\033[2J",                  \
       sizeof("\033[H\033[2J"))) != sizeof("\033[H\033[2J"))  \
  {                                                           \
    perror("write: clear screen");                            \
    exit(EXIT_FAILURE);                                       \
  }                                                           \
} while(0);                                                   \

static char *s, c;
static size_t sl, n, cur;
static struct termios oldt, newt;

static inline 
void setrawmode()
{
  tcgetattr(STDIN_FILENO, &oldt);    
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void cleanup()
{
  putchar('\n');
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

static inline
void putchar_at_offset(size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        exit(EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC%c", offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        exit(EXIT_FAILURE);
      }
  }
}

int 
main()
{
  atexit(cleanup);
  setrawmode();

  clscn();

  s = "this is the string\n";
  sl = strlen(s);

  n = write(STDOUT_FILENO, s, sl);
  if (n != sl) exit(EXIT_FAILURE);

  cur = 0;

  while (1)
  {
    if (cur == sl - 1) break;

    n = read(STDIN_FILENO, &c, 1);
    if (n != 1) {
      perror("read STDIN");
      exit(EXIT_FAILURE);
    }

    if (c != s[cur]) continue;

    putchar_at_offset(cur, c);
    fflush(stdout);

    ++cur;
  }

  exit(EXIT_SUCCESS);
}
