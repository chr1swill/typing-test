#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "american_english.h"

static inline
void count_linefeed(size_t *i, size_t *counter, char *bytes, size_t bytelen)
{
  *i = 0;
  *counter = 0;

  for (;*i < bytelen; ++(*i))
  {
    switch (bytes[(*i)])
    {
      case '\n': ++(*counter); break;
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
  size_t i, j;
  char *cursor;

  count_linefeed(&i, &sp->len, bytes, byteslen);

  sp->strarr = malloc(sizeof(char *) * sp->len);
  if (sp->strarr == NULL)
  {
    perror("StrSpan_from_linefeed_delim_bytes: malloc");
    exit(EXIT_FAILURE);
  }

  i = 0;
  j = 0;

  cursor = &bytes[i];
  for (;i < byteslen; ++i)
  {
    switch (bytes[i])
    {
      case '\n': 

        sp->strarr[j] = cursor;

        cursor = &bytes[i+1];
        bytes[i] = '\0';
        ++j;
      break;
      default: continue;
    }
  }
}

// clean up the stuff inside struct
// but you take care of the struct
// since you are responsible for providing it (allocating)
static inline
void StrSpan_empty(StrSpan *sp)
{
  free(sp->strarr);
}

int main(int argc, char **argv)
{
  int r;
  size_t i;
  StrSpan sp;

  if (argc < 2)
  {
    printf("brother fix that\n");
    return 1;
  }

  i = atol(argv[1]);
  printf("words_in_test as number=%ld\n", i);
  
  int randoz[i];
  const size_t randoz_len = i;

  StrSpan_from_linefeed_delim_bytes(&sp,
    american_english, american_english_len);
  printf("sp.len=%ld\n", sp.len);

  srand(time(NULL) - randoz_len);

  i = 0;
  for (; i < randoz_len; ++i)
  {
    randoz[i] = rand() % (sp.len - 1);
  }

  i = 0;
  for (;i < randoz_len; ++i)
  {
    printf("sp.strarr[%d]=%s\n", randoz[i], sp.strarr[randoz[i]]);
  }

  fflush(stdout); 
  StrSpan_empty(&sp);
  return 0;
}
