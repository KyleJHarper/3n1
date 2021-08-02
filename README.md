Wondering if there's a way to optimize the brute-force techniques employed to test the 3n+1 (collatz) testing.

As of this writing (2021) the space tested was 0 - 2^68 (2.95 x 10^20, or simply 2.95e20), and the documented rates (in tests/sec) of
computation were:
Authors | Sieve(1) | Numbers(2) | Speed | Hardware | Architecture
------- | -------- | ---------- | ----- | -------- | ------------
Honda et al. | 2^37 | 64-bit | 1.31 e12 | GTX Titan X | GPU
Honda et al. | 2^37 | 64-bit | 5.25 e9 | i7-4790 | CPU
Roosendaal | 2^32 | 64-bit | 4.63 e8 | Contemporary CPUs(3) | CPU
Oliveira et al. | 2^46 | 64-bit | 2.25 e9 | CPUs circa 2004-2009 | CPU
Barina | 2^34 | 128-bit | 4.21 e9 | Xeon Gold 5218 | CPU
Barina | 2^24 | 128-bit | 2.20 e11 | RTX 2080 | GPU

(1) I'm guessing the sieve was for Base2?  At this point, I haven't delved into sieves yet.

(2) Means width.

(3) No idea what this means.


(Details at: https://www.semanticscholar.org/paper/Convergence-verification-of-the-Collatz-problem-Barina/0dc674cb2c6d9a7d85621d77287fb009458eeaa9/figure/0)

# Big Disclaimer
I am not a mathematician.

I am a computer engineer.  This goal is learning.

The general approach to brute forcing isn't actually a brute force.  Very smart people are using sieves and other techniques to avoid
testing numbers that can be mathematically proved to end back at 1.  My aim is to test the optimizations themselves and generate data to
identify which optimizations are most efficient (highest rejections per unit of CPU time).  A natural byproduct of this approach will be an
ever-faster method of testing 3n1 spaces.  I believe there are other optimizations that may have been overlooked which are computer-related
rather than math-related.

tl;dr - This is (currently) a hobby project.

## Potential Uselessness
It should be noted that this endeavor is possibly useless.  Even if we are able to test the space beyond 2^68, if it doesn't mean we'll
find anything.

If we find nothing (no counterexample), we still haven't proven anything.

The goal of this project is as much about optimization as it is about an old math conjecture.

# Areas of focus
## Breakdown Of Optimization Cost
Optimizations come at a cost.  Consider two optimizations, O-1 and O-2.  If they possess equal rejection rates and computation cost, then
they can be used in any order.  But if one offers more rejection percentage per unit of computation, it is superior (assuming short-circuit
logic, which is reasonable).

Example:
* Givens: a set of N terms is 100 and the cost to compute is 20 units.  O-1 and O-2 are 10 units each.
* O-1 will be applied to all 100 terms, costing 1000 units.
* O-2 will only be applied to 10 terms, costing 100 units.
* Only 1 term will survive optimization, costing 20 units.

Total Cost: 1120.

Example 2:
* Same as first, except O-2 only costs 5 units but its rejection rate is 70%.
* O-1 applies to 100 terms, rejects 90, and still costs 1000.
* O-2 applies to 10 terms, rejects 7, and costs 50.
* 3 terms survive and cost 60.

Total Cost: 1100

Example 2-1:
* Same as example 2, except we apply O-2 first.
* O-2 applies to 100 terms, rejects 70, and costs 500.
* O-1 applies to 30 terms, rejects 90%, and costs 300.
* 3 terms survive again, costing 60.

Total Cost: 860

In the above examples, if we'd simply brute-forced every term, it would have cost 100 * 20 == 2000 units.  Our "90%" optimization does not
mean we reduce our computational load by 90%.  In fact, the more time we commit to processing an optimization, the higher our "floor"
raises.  E.g.: an optimization that is 100% efficient and costs 5 units will never be faster than "500".  Computers are extremely sensitive
to cache misses and many optimizations center around cache hit ratios, cache (line) optimization, avoiding RAM, branch mispredictions,
pipeline flushing, etc.

As an added spur in my butt, the efficiency of these optimizations may change as the number of terms increases or the terms become large
numbers.  Even if an optimization is mathematically guaranteed (not just probabilistic), the amount of work a computer has to do may change
(e.g.: losing a microcode optimization when term N grows from a 64-bit item to a 128-bit).

Therefore, our first area of focus is to study how efficient optimizations are, and to decide if there are methods for grouping up the work
into tighter, higher-throughput regions.

## Existing Optimizations and Techniques
These are techniques already in use, or at least mentioned by others, which I need to look into.

* Sieves.  I've seen 2-mod, 3-mod, and others, but I admittedly don't know what they are yet.  I believe these are time-memory tradeoffs,
which I do have experience with, but lack the exposure to here.
* Hardware.  Other architectures (such as GPUs) offer great parallelization efficiency.  I'm guessing an FPGA or ASIC would be even better,
but would cost a fortune of course.
* The author at https://github.com/hellpig/collatz has a lot of techniques documented--with lots of exclamation points!!!! :)

## Random Ideas From My Brain
The following are considerations I'm toying with.  I'm sure many (possibly all) of these have been considered by others.

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
