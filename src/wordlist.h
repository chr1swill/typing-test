#ifndef _WORDLIST_H
#define _WORDLIST_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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
 
void StrSpan_make(StrSpan *ss);

void StrSpan_free(StrSpan *sp);

void count_linefeed(size_t *i, size_t *counter,
                    char *bytes, size_t bytelen);

void StrSpan_from_linefeed_delim_bytes(StrSpan *ss,
                                       char *bytes, size_t byteslen);
typedef struct
{
  char *bytes;
  size_t len;
} Span;

void Span_make(Span *span, size_t len, char c);

void Span_free(Span *span);

void make_wordlist(Span *span, StrSpan *ss,
                   int *randnums, size_t randnums_len);

void gen_random_wordlist(Span *s, StrSpan *ss, size_t *n_words);
#endif // _WORDLIST_H
