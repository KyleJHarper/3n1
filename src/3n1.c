#include <stdio.h>
#include <inttypes.h>
#include "helpers.h"
#include "error.h"
#include "options.h"



int inefficient(uint128_t val) {
  // The conjecture states we must start with 3x+1, but most implementations I see keep looping over an if..else block.
  // We could add an assumption about 3x+1 at the top of a while() loop, but we'll purposely use the if..else method others do for testing.
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



int main(int argc, char **argv) {
  // Let's make it work first.
  Options opts = process_options(argc, argv);
  print128(opts.start);
  //printf("Start: %"PRIu64"  End: %"PRIu64"\n", opts.start, opts.end);

  // Single Test
  if(opts.mode == SINGLE_MODE) {
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
