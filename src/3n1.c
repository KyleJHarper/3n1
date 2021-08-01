#include <stdio.h>
#include <inttypes.h>
#include "helpers.h"
#include "error.h"
#include "options.h"



//
// A basic, inefficient way to test a single number.  This is the loop I see most people using, when they're not busy abusing recursion ...
int inefficient(uint128_t val) {
  while(val != 1) {
    if(val % 2 == 0) {
      // It's even, so divided by 2.
      val /= 2;
    } else {
      // It's odd, so multiply again.
      val = 3 * val + 1;
    }
  }
  // We found the path to 1, so return okay.
  return 0;
}


//
// Perform a single test.  We don't care about performance here.
void single(uint128_t val) {
  int multiplications = 0;
  int divisions = 0;
  print128(val);
  while(val != 1) {
    if(val % 2 == 0) {
      val /= 2;
      divisions++;
    } else {
      val = 3 * val + 1;
      multiplications++;
    }
    printf(" => ");
    print128(val);
    fflush(stdout);
  }
  printf("\nEnded at 1 after %d steps (%d multiplications, %d divisions).\n", multiplications + divisions, multiplications, divisions);
  return;
}



// TODO:  Since we're using power-of-two modulo, consider: x % 2n ==> x & (2n - 1), or in our case: x % 2 ==> x & 1
int main(int argc, char **argv) {
  // Let's make it work first.
  Options opts = process_options(argc, argv);
  printf("Start is ");
  print128(opts.start);
  printf(".  End is ");
  print128(opts.end);
  printf("\n");

  // Single Test
  if(opts.mode == SINGLE_MODE) {
    printf("Performing a SINGLE_MODE test.\n");
    single(opts.start);
    exit(0);
  }

  // Loop through our range.
  //printf("Testing %"PRIu64" -> %"PRIu64"\n", opts.start, opts.end);
  for (uint64_t i = opts.start; i <= opts.end; i++) {
    if(inefficient(i) != 0)
      printf("We found a counterexample at %"PRIu64".", i);
  }


  exit(0);
}
