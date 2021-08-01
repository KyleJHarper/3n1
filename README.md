Stand-in for a TODO list currently.

Wondering if there's a way to optimize the brute-force techniques employed to test the 3n+1 (collatz) testing.

As of this writing (2021) the space tested was 0 - 2^68 (2.95 x 10^20, or simply 2.95e20), and the documented rates (in tests/sec) of computation were:
* 1.31 x 10^12  (GPU-based, 64-bit int)
* 2.20 x 10^11  (GPU-based, 128-bit int)
* 5.25 x 10^ 9  (x86 CPU, 64-bit)
* 4.21 x 10^ 9  (x64 CPU, 128-bit)

(Details at: https://www.semanticscholar.org/paper/Convergence-verification-of-the-Collatz-problem-Barina/0dc674cb2c6d9a7d85621d77287fb009458eeaa9/figure/0)

# Potential Uselessness
It should be noted that this endeavor is possibly useless.  Even if we are able to test the space beyond 2^68, if it doesn't mean we'll find anything.

If we find nothing (no counterexample), we still haven't proven anything.

The goal of this project is as much about optimization as it is about an old math conjecture.

### Areas of focus
* Stop recursing, it's inefficient.
* Minimize memory writing.  Stop tracking steps and max values and other crap we don't care about for space testing.
* Parallelize the work, each test (number) is atomic.  And our assumption below (about testing lower numbers) is proved when each block completes.
* Stop testing down to 1, there are better sentinal tests.  E.g.: we only need to test if we've hit a power of two.
* Don't process numbers we've already tested below "x".  E.g.: 3x+1 always gets halved, and if its even again, we're done, we don't even need to halve it (we've tested that number).
* I'm not convinced we need to test even starting numbers.  If I test 1-10 manually, then test 11-20, I know that 12, 14, 16, 18, and 20 will all boil down to
values I've already confirmed (6, 7, 8, 9, and 10, respectively).  When I move on to 21-30, same: 22, 24, 26, 28, and 30 have been tested either directly in
the previous pass because they were odds (11, 13, 15) or by virtue (12, 14 which were tested via 6, 7).  In short, as long as we've scanned all the odds
leading up to N, we've inevitably scanned all the evens (N/2) _and_ all the evens of double that value: 2N (because 2N is always even which immediately
results in us dividing by 2, taking us back to N).
* Treat all numbers that are multiplied by 3x+1 as a group of "initialization points", and upon discovering we reach 1, they will ALL reach 1. E.g.:
  * Start with 7 (put 7 in IP group => [7])
  * 7 * 3 + 1 == 22
  * 22 / 2 == 11 (not even, add to IP group => [7, 11])
  * 11 * 3 + 1 == 34
  * 34 / 2 == 17 (not even, add to IP group => [7, 11, 17])
  * 17 * 3 + 1 == 52
  * 52 / 2 == 26
  * 26 / 2 == 13 (not even, add to IP group => [7, 11, 13, 17])
  * 13 * 3 + 1 == 40
  * 40 / 2 == 20
  * 20 / 2 == 10
  * 10 / 2 == 5 (not even, add to IP group => [5, 7, 11, 13, 17]) (also, we could stop here since 5 < x, see above)
  * 5 * 3 + 1 == 16  (we could stop here because 16 is a power of two, see above)
  * 16 / 2 == 8
  * 8 / 2 == 4
  * 4 / 2 == 2
  * 2 / 2 == 1  (STOP)
  * All numbers in our IP group will reach 1 eventually: [5, 7, 11, 13, 17].  They do not need tested individually.
  * Note: this might become inefficient to track as numbers get larger and as duplicates are found from previous iterations of X.  However
    we might avoid this if we chunk up the ranges reasonably and put them in a sorted datastructure (vla-ish, skiplist, etc) that will give us
    reasonable search performance (reasonable meaning, better to check each time than brute force everything).
