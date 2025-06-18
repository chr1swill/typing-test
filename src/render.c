#include "render.h"

void setrawmode(struct termios *oldt, struct termios *newt)
{
  tcgetattr(STDIN_FILENO, oldt);
  *newt = *oldt;

  newt->c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

void cleanup(struct termios *oldt)
{
  tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

void clean_on_exit(struct termios *oldt, int status)
{
  putchar('\n');
  cleanup(oldt);
  exit(status);
}

void putchar_at_offset_white(struct termios *oldt, size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B\033[40m%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC\033[40m%c",
           offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
  }

  fflush(stdout);
}

void putchar_at_offset_red(struct termios *oldt, size_t offset, char c)
{
  switch (offset)
  {
    case 0:
      if ((printf("\033[H\033[B\033[41m%c", c)) == -1)
      {
        perror("putchar_at_offset: offset 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
    break;
    default:
      if ((printf("\033[H\033[B\033[%zuC\033[41m%c",
           offset, c)) == -1)
      {
        perror("putchar_at_offset: offset greater than 0");
        clean_on_exit(oldt, EXIT_FAILURE);
      }
  }

  fflush(stdout);
}
