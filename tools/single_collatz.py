#!/usr/bin/python3
import sys


# Helper to calculate collatz as a string.
def generate_collatz(num: int) -> str:
    stops = [str(num)]
    while num > 1:
        if num % 2 == 0:
            num = int(num / 2)
        else:
            num = num * 3 + 1
        stops.append(str(num))
    result = ', '.join(stops)
    return result


print(generate_collatz(int(sys.argv[1])))
