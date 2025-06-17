#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

typedef struct
{
  char *bytes;
  size_t len;
} Span;

static inline
void Span_make(Span *span, size_t len, char c)
{
  span->bytes = malloc(sizeof(char) * len);
  if (span->bytes == NULL)
  {
    perror("Span_new: malloc");
    exit(EXIT_FAILURE);
  }
  memset(span->bytes, c, len);

  span->len = len;
}

static inline
void Span_free(Span *span)
{
  free(span->bytes);
}

// user allocated, uninited Span
// full StrSpan
static inline
void make_wordlist(Span *span, StrSpan *ss,
                   int *randnums, size_t randnums_len)
{
  size_t i, str_len; 
  char *str;
  char *cursor;

  i = 0;
  for (;i < randnums_len; ++i)
  {
    span->len += strlen(ss->strarr[randnums[i]]);
  }

  // for the spaces in between words
  //span->len += (randnums_len - 1); 
  span->len += randnums_len; 

  Span_make(span, span->len, ' ');
  //printf("span->len=%ld\n", span->len);

  i = 0;
  cursor = &span->bytes[0]; 
  for (;i < randnums_len; ++i)
  {
    str = ss->strarr[randnums[i]];
    str_len = strlen(str);

    memmove(cursor, str, str_len); 
    cursor += str_len + 1;
    // if (i != randnums_len)
    // {
    //   cursor += str_len + 1;
    // }
    // else
    // {
    //   cursor[str_len + 1] = '\n';
    // }
  }

  span->bytes[span->len - 1] = '\n';

  assert(span->len == (cursor - span->bytes));
} 

#define usage() \
do { \
  printf("usage: %s [number of words in test]\n"        \
          "example: %s 4 // creates a 4 word test\n"    \
          "exmaple: %s 50 // creates a 50 word test\n", \
          argv[0], argv[0], argv[0]);                   \
  exit(EXIT_FAILURE);                                   \
} while(0); \

int main(int argc, char **argv)
{
  int r;
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

  //printf("words_in_test as number=%ld\n", i);
  
  int randoz[i];
  const size_t randoz_len = i;

  StrSpan_from_linefeed_delim_bytes(&ss,
    american_english, american_english_len);
  //printf("ss.len=%ld\n", ss.len);

  srand(time(NULL) - randoz_len);

  i = 0;
  for (; i < randoz_len; ++i)
  {
    randoz[i] = rand() % (ss.len - 1);
  }

  make_wordlist(&s, &ss, randoz, randoz_len);

  write(STDOUT_FILENO, s.bytes, s.len);
  fflush(stdout); 

  StrSpan_empty(&ss);
  Span_free(&s);
  return 0;
}
