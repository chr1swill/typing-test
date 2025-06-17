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
void Span_make(Span *span, size_t len)
{
  span->bytes = malloc(sizeof(char) * len);
  if (span->bytes == NULL)
  {
    perror("Span_new: malloc");
    exit(EXIT_FAILURE);
  }
  memset(span->bytes, 0, len);

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
  size_t i; 
  char *str;
  char *cursor;

  i = 0;
  for (;i < randnums_len; ++i)
  {
    span->len += strlen(ss->strarr[randnums[i]]);
  }

  // for the spaces in between words
  span->len += (randnums_len - 1); 

  Span_make(span, span->len);
  printf("span->len=%ld\n", span->len);

  i = 0;
  cursor = &span->bytes[0]; 
  for (;i < randnums_len; ++i)
  {
    str = ss->strarr[randnums[i]];

    memmove(cursor, str, strlen(str)); 
    cursor += strlen(str) + 1;
  }

  assert(span->len == (cursor - span->bytes) - 1);
} 

int main(int argc, char **argv)
{
  int r;
  size_t i;
  StrSpan ss;
  Span s;

  if (argc < 2)
  {
    printf("brother fix that\n");
    return 1;
  }

  i = atol(argv[1]);
  printf("words_in_test as number=%ld\n", i);
  
  int randoz[i];
  const size_t randoz_len = i;

  StrSpan_from_linefeed_delim_bytes(&ss,
    american_english, american_english_len);
  printf("ss.len=%ld\n", ss.len);

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
