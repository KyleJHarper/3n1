#include <stdio.h>
#include <inttypes.h>
#include "helpers.h"


//
// We cannot convert a string to a 128-bit integer with native ato-* functions.  Write our own (by stealing it from the internet).
int128_t atoi128(const char *s)
{
  while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '+') ++s;
  int sign = 1;
  if (*s == '-') {
    ++s;
    sign = -1;
  }
  size_t digits = 0;
  while (s[digits] >= '0' && s[digits] <= '9') ++digits;
  char scratch[digits];
  for (size_t i = 0; i < digits; ++i) scratch[i] = s[i] - '0';
  size_t scanstart = 0;

  int128_t result = 0;
  int128_t mask = 1;
  while (scanstart < digits)
  {
    if (scratch[digits-1] & 1) result |= mask;
    mask <<= 1;
    for (size_t i = digits-1; i > scanstart; --i)
    {
      scratch[i] >>= 1;
      if (scratch[i-1] & 1) scratch[i] |= 8;
    }
    scratch[scanstart] >>= 1;
    while (scanstart < digits && !scratch[scanstart]) ++scanstart;
    for (size_t i = scanstart; i < digits; ++i)
    {
      if (scratch[i] > 7) scratch[i] -= 3;
    }
  }

  return result * sign;
}


//
// Print a 128bit int.
int print128(int128_t n) {
  if (n == 0)  return printf("0\n");

  uint64_t factor = 10000000000000; // 3*13 == 39: 2**128 < 10**39
  const int size = 3;
  uint64_t parts[size];

  uint64_t *p = parts + size; // start at the end
  while (p != parts) {
    *--p = n % factor; // get last part
    n /= factor;       // drop it
  }

  for (p = parts; p != &parts[size]; ++p)
    if (*p) {
      printf("%" PRIu64, *p);
      break; // found nonzero part
    }

  for ( ++p; p != &parts[size]; ++p)
    printf("%013" PRIu64, *p);

  return 0;
}