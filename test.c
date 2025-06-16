#include <stdlib.h>
#include <stdio.h>
#include "american_english.h"
#include <unistd.h>

static inline
void count_linefeed(size_t *counter, char *bytes, size_t bytelen)
{
  size_t i;

  i = 0;
  *counter = 0;

  for (;i < bytelen; ++i)
  {
    switch (bytes[i])
    {
      case '\n': ++(*counter);
      default: continue;
    }
  }
}

typedef struct {
  char **strarr;
  size_t len;
} StrSpan;

void StrSpan_from_linefeed_delim_bytes(StrSpan *sp, char *bytes, size_t byteslen)
{
  count_linefeed(&sp->len, bytes, byteslen);
}

// clean up the stuff inside struct
// but you take care of the struct
// since you are responsible for providing it (allocating)
static inline
void StrSpan_empty(StrSpan *sp)
{
  while (sp->len != 0)
  {
    --sp->len;
    free(sp->strarr[sp->len]);
  }
}

int main()
{
  //int r, words_in_test;

  //if (argc < 2)
  //{
  //  printf("brother fix that\n");
  //  return 1;
  //}

  //const char *input_words_in_test = argv[1];

  //words_in_test = atoi(input_words_in_test);
  //printf("words_in_test as number=%d\n", words_in_test);
  //
  //int randoz[words_in_test];
  //const size_t randoz_len = words_in_test;

  //for (words_in_test = 0; words_in_test < randoz_len; ++words_in_test)
  //{
  //  randoz[words_in_test] = rand();
  //  printf("%d:%d\n", words_in_test, randoz[words_in_test]);
  //}

  //fflush(stdout); 

  StrSpan sp;

  StrSpan_from_linefeed_delim_bytes(&sp,
    american_english, american_english_len);

  printf("sp.len=%ld\n", sp.len);

  // StrSpan_empty(&sp);
  return 0;

  //printf("RAND_MAX=%d\n\n", RAND_MAX);

  //r = rand();
  //printf("%d\n", r);

  //r = rand();
  //printf("%d\n", r);

  //r = rand();
  //printf("%d\n", r);

  //r = rand();
  //printf("%d\n", r);

  //r = rand();
  //printf("%d\n", r);

  //r = rand();
  //printf("%d\n", r);

  //return 0;
}
