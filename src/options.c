#include <stdio.h>        /* for fprintf and stderr */
#include <unistd.h>       /* for getopt */
#include <string.h>       /* for strlen */
#include "options.h"
#include "helpers.h"

#define START_DEFAULT  1
#define END_DEFAULT    1000000
#define UINT128_MAX    340282366920938463463374607431768211456

//
// process_options
Options process_options(int argc, char **argv) {
  // Get an options struct, set it based on args, and return it.
  Options opts;

  /* Set up defaults. */
  opts.start = START_DEFAULT;
  opts.end = END_DEFAULT;

  /* Process everything passed from CLI now. */
  int c = 0;
  opterr = 0;
  while ((c = getopt(argc, argv, "e:hm:s:")) != -1) {
    switch (c) {
      case 'e':
        opts.end = atoi128(optarg);
        break;
      case 'm':
        opts.mode = (uint64_t)atoll(optarg);
        break;
      case 's':
        opts.start = atoi128(optarg);
        break;
      case '?':
        show_help();
        if (optopt == 'e' || optopt == 's')
          show_error(1, "Option -%c requires an argument.", optopt);
        if (isprint (optopt))
          show_error(1, "Unknown option `-%c'.", optopt);
        show_error(1, "Unknown option character `\\x%x'.\n", optopt);
        break;
      default:
        show_help();
        exit(1);
    }
  }

  /* Pre-flight Checks */
  if (opts.start < 1)
    show_error(1, "You cannot set the 'start' value to anything lower than 1.  We don't test negative integers.");
  if (opts.end < opts.start && opts.mode != SINGLE_MODE)
    show_error(1, "You cannot specify an 'end' value that is lower than the 'start' value.");
  return opts;
}


/* show_help
 * Spits out the basic syntax for tyche.
 */
void show_help() {
  fprintf(stderr, "\n");
  fprintf(stderr, "3n1 - Brute force program to test a larger range of integers by optimizing the approach.\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "  Usage: ./prog_name -s <start> -e <end>\n");
  fprintf(stderr, "     ex: ./prod_name -s 1 -e 1000000\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "  Options:\n");
  fprintf(stderr, "    %2s   %-13s   %s", "-e", "<number>",       "End value to test in space.  Default 1000000.\n");
  fprintf(stderr, "    %2s   %-13s   %s", "-h", "",               "Show this help.\n");
  fprintf(stderr, "    %2s   %-13s   %s", "-s", "<number>",       "Start value to test in space.  Default 1.\n");
  fprintf(stderr, "\n");

  return;
}
