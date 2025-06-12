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
void putchar_at_offset_white(size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B\033[97m%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        exit(EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC\033[97m%c",
           offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        exit(EXIT_FAILURE);
      }
  }

  fflush(stdout);
}

static inline
void putchar_at_offset_red(size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B\033[31m%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        exit(EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC\033[31m%c",
           offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        exit(EXIT_FAILURE);
      }
  }

  fflush(stdout);
}

static inline
void putchar_at_offset(size_t offset, char c, short bool_is_error)
{
  char writebuf[32];
  char *pointer;
  size_t slen;
  int rc;
  
  pointer = &writebuf[0];

  #define str "\033[H\033[B"
  slen = strlen(str);
  memmove(&pointer, str, slen);
  pointer += slen;
  #undef str
  
  if (offset != 0)
  {
    #define str "\033[%zuC"
    slen = strlen(str);

    rc = snprintf(pointer, slen, str, offset);
    if (rc != (int)slen)
    {
      perror("putchar_at_offset: snprintf");
      exit(EXIT_FAILURE);
    }
    #undef str

    pointer += slen;
  }

  if (bool_is_error == 1) //TRUE
  {
    #define str "\033[31m"
    slen = strlen(str);

    memmove(pointer, str, slen);
    pointer += slen;
    #undef str
  }
  else
  {
    #define str "\033[97m"
    slen = strlen(str);

    memmove(pointer, str, slen);
    pointer += slen;
    #undef str
  }
  
  memmove(pointer, &c, 1);
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

    if (c != s[cur])
    {
      putchar_at_offset_red(cur, c);
    }
    else
    {
      putchar_at_offset_white(cur, c);
      ++cur;
    }
  }

  exit(EXIT_SUCCESS);
}
