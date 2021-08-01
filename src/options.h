#ifndef SRC_OPTIONS_H_
#define SRC_OPTIONS_H_

/* Includes */
#include <stdint.h>
#include <stdlib.h>  /* for atoi family */
#include <ctype.h>   /* for isprint */
#include "error.h"
#include "helpers.h"

#define DEFAULT_MODE  0   // Inefficient mode.
#define SINGLE_MODE   1   // Test and print a single path.


/* A Convenient Struct to hold our options and settings. */
typedef struct options Options;
struct options {
  /* Manager Tracking */
  // uint64_t start;  // First number to start testing.
  uint128_t start;  // First number to start testing.
  uint128_t end;    // Last number to test.
  uint8_t mode;     // The mode we want to operate in.
  // /* Page Information */
  // char *page_directory;         // The root directory for pages to find pages.
  // uint32_t page_count;          // The number of pages found when scanning for data to use.
  // uint32_t page_limit;          // The limit imposed when scanning page_directory for pages.
  // uint16_t smallest_page;       // Size of the smallest page found while scanning.
  // uint16_t biggest_page;        // Size of the largest page found while scanning.
  // uint64_t dataset_size;        // Total bytes in the pages scanned, respecting of page_limit.
  // uint64_t dataset_max;         // The maximum size, in bytes, for all pages in the dataset.

  // /* Resource Control */
  // uint64_t max_memory;          // Maximum amount of memory tyche can use for buffers.
  // int8_t fixed_ratio;           // If non-negative, enforce raw list to this ratio when balancing.
  // uint16_t workers;             // Number of worker threads to use simultaneously.
  // uint16_t cpu_count;           // Number of CPUs/cores available.

  // /* Tyche Management */
  // uint16_t duration;            // Amount of time for each worker to run, in seconds (s).
  // int compressor_id;            // The ID of the compressor to use for buffer__compress/decompress.
  // int compressor_level;         // The level of zlib/zstd to use (1-9).  Future option.  For now, always 1.
  // int min_pages_retrieved;      // The minimum number of pages to find and pin for a "round" in a worker.
  // int max_pages_retrieved;      // The maximum number of pages to find and pin for a "round" in a worker.
  // float bias_percent;           // Percentage of data set that is most popular (e.g.: 20%)
  // float bias_aggregate;         // Percentage of all hits that the biased buffers represent (e.g.: 80%)
  // float update_frequency;       // Percentage of hits that should result in a list__update() too.
  // float delete_frequency;       // Percentage of hits that should result in a list__remove() too.  Generally VERY small.

  // /* Run Test? */
  // char *test;                   // Name of a test to run.
  // char *extended_test_options;  // Extended options required for some tests.

  // /* Niceness Features */
  // uint8_t quiet;                // Should we suppress most output.  0 == Normal, 1 == Quiet.
  // uint8_t verbosity;            // How noisy to be when we're not being quieted.
};


/* Prototypes, for science! */
Options process_options(int argc, char **argv);
void show_help();

#endif /* SRC_OPTIONS_H_ */