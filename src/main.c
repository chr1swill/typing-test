#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "wordlist.h"

int main(int argc, char **argv)
{
  char c;
  Span s;
  StrSpan ss;
  size_t i, n, cur;
  struct termios oldt, newt;

  if (argc < 2)
  {
    usage();
  }

  i = atol(argv[1]);
  if (i < 1)
  {
    usage();
  }

  setrawmode(&oldt, &newt);
  clscn();

  gen_random_wordlist(&s, &ss, &i);

  n = write(STDOUT_FILENO, s.bytes, s.len);
  if (n != s.len) {
    clean_on_exit(&oldt, EXIT_FAILURE);
  }

  cur = 0;
  while (1)
  {
    if (cur == s.len - 1) break;

    n = read(STDIN_FILENO, &c, 1);
    if (n != 1) {
      perror("read STDIN");
      clean_on_exit(&oldt, EXIT_FAILURE);
    }

    if (c != s.bytes[cur])
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
