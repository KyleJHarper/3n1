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

