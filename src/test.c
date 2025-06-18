#include "wordlist.h"

int main(int argc, char **argv)
{
  size_t i;
  StrSpan ss;
  Span s;

  if (argc < 2)
  {
    usage();
  }

  i = atol(argv[1]);
  if (i < 1)
  {
    usage();
  }

  gen_random_wordlist(&s, &ss, &i);

  write(STDOUT_FILENO, s.bytes, s.len);
  fflush(stdout); 

  StrSpan_free(&ss);
  Span_free(&s);
  return 0;
}
