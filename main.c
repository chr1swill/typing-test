#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "render.h"

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
