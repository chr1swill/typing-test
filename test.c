#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "american_english.h"

#define usage() \
do { \
  printf("usage: %s [number of words in test]\n"        \
          "example: %s 4 // creates a 4 word test\n"    \
          "exmaple: %s 50 // creates a 50 word test\n", \
          argv[0], argv[0], argv[0]);                   \
  exit(EXIT_FAILURE);                                   \
} while(0); \

typedef struct {
  char **strarr;
  size_t len;
} StrSpan;

static inline
void StrSpan_make(StrSpan *ss)
{
  ss->strarr = malloc(sizeof(char *) * ss->len);
  if (ss->strarr == NULL)
  {
    perror("StrSpan_make: malloc");
    exit(EXIT_FAILURE);
  }
}

static inline
void StrSpan_empty(StrSpan *sp)
{
  free(sp->strarr);
}

static inline
void count_linefeed(size_t *i, size_t *counter,
                    char *bytes, size_t bytelen)
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

void StrSpan_from_linefeed_delim_bytes(StrSpan *ss,
                                        char *bytes, size_t byteslen)
{
  size_t i, j;
  char *cursor;

  count_linefeed(&i, &ss->len, bytes, byteslen);
  StrSpan_make(ss);

  i = 0;
  j = 0;

  cursor = &bytes[i];
  for (;i < byteslen; ++i)
  {
    switch (bytes[i])
    {
      case '\n': 

        ss->strarr[j] = cursor;

        cursor = &bytes[i+1];
        bytes[i] = '\0';
        ++j;
      break;
      default: continue;
    }
  }
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
    perror("Span_make: malloc");
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

static inline
void make_wordlist(Span *span, StrSpan *ss,
                   int *randnums, size_t randnums_len)
{
  size_t i, str_len; 
  char *str;
  char *cursor;

  i = 0;
  span->len = 0;
  for (;i < randnums_len; ++i)
  {
    span->len += strlen(ss->strarr[randnums[i]]);
  }

  span->len += randnums_len; 
  Span_make(span, span->len, ' ');

  i = 0;
  cursor = &span->bytes[0]; 
  for (;i < randnums_len; ++i)
  {
    str = ss->strarr[randnums[i]];
    str_len = strlen(str);

    memmove(cursor, str, str_len); 
    cursor += str_len + 1;
  }

  span->bytes[span->len - 1] = '\n';
  assert(span->len == (cursor - span->bytes));
} 

static inline
void gen_random_wordlist(Span *s, StrSpan *ss, size_t *n_words)
{
  const size_t randoz_len = *n_words;
  int randoz[randoz_len];

  StrSpan_from_linefeed_delim_bytes(ss,
    american_english, american_english_len);

  srand(time(NULL) - randoz_len);

  *n_words = 0;
  for (; *n_words < randoz_len; ++(*n_words))
  {
    randoz[(*n_words)] = rand() % (ss->len - 1);
  }

  make_wordlist(s, ss, randoz, randoz_len);
}

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

  StrSpan_empty(&ss);
  Span_free(&s);
  return 0;
}
