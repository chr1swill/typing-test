//#include <stdlib.h>
//#include <stdio.h>
#include "american_english.h"
#include <unistd.h>

static inline
void count_linefeed(size_t *counter, char *bytes, size_t bytelen)
{
  size_t i;

  i = 0;
  *strarr_len = 0;

  for (;i < bytesteam_len; ++i)
  {
    switch (bytestream[i])
    {
      case '\n': ++(*strarr_len);
      default: continue;
    }
  }
}

char **strarr_from_linefeed_bytestream(size_t *strarr_len,
                                        char *bytes,
                                        size_t byteslen)
{
  void count_linefeed(strarr_len, bytes, byteslen)
  
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

  write(STDOUT_FILENO, &american_english, american_english_len);
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
