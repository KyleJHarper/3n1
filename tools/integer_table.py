#!/usr/bin/python3
import argparse

"""
Builds a pretty table of integers from --start to --end and shows binary and hex formats.
"""

# Get args
parser = argparse.ArgumentParser(description="Build a table of integers in decimal, binary, and hex.")
parser.add_argument(
    '--start',
    type=int,
    help="Starting number.  Default is %(default)s.",
    default=0,
    action='store',
    dest='start',
)
parser.add_argument(
    '--end',
    type=int,
    help="Ending number.  Default is %(default)s.",
    default=20,
    action='store',
    dest='end',
)
parser.add_argument(
    '--binary-padding',
    type=int,
    help="How many leading zeros to show in binary (for ease of reading).  Default is %(default)s.",
    default=4,
    action='store',
    dest='binary_padding',
)
parser.add_argument(
    '--binary-split',
    type=int,
    help="How many bits to group together before spacing.  Default is %(default)s.",
    default=8,
    action='store',
    dest='binary_split',
)
parser.add_argument(
    '--breakup-size',
    type=int,
    help="Break the results with an empty row every X rows.  Default is %(default)s.",
    default=20,
    action='store',
    dest='breakup_size',
)
parser.add_argument(
    '--collatz-width',
    type=int,
    help="Show up to X characters of the collatz conjecture.  Default is %(default)s.",
    default=40,
    action='store',
    dest='collatz_width',
)
args = parser.parse_args()
start = args.start
end = args.end
binary_padding = args.binary_padding
binary_split = args.binary_split
breakup_size = args.breakup_size
collatz_width = args.collatz_width
binary_group_spacing = 3


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


# Calculate maximum bit size and column widths.
max_bits = 1
while (pow(2, max_bits) < end):
    max_bits += 1
# Pad a few for clarity in the table.
max_bits = 64 if (max_bits + binary_padding) > 64 else max_bits + binary_padding
group_count = int(max_bits/binary_split) + (0 if max_bits % binary_split == 0 else 1)
decimal_width = len(str(end)) if len(str(end)) >= 3 else 3
hex_width = len(hex(end)) if len(hex(end)) >= 3 else 3
collatz_width = max(collatz_width, 10)
binary_width = (group_count * binary_split) + ((group_count - 1) * binary_group_spacing)

# Build the headers and separators.
separator = f"+-{'-' * decimal_width}-+-{'-' * hex_width}-+-{'-' * binary_width}-+-{'-' * collatz_width}-+"
decimal_header = f"{'Decimal'[:decimal_width]:<{decimal_width}}"
hex_header = f"{'Hex'[:hex_width]:<{hex_width}}"
collatz_header = f"{'Collatz'[:collatz_width]:<{collatz_width}}"
binary_header = f"{'Binary'[:binary_width]:<{binary_width}}"
binary_key_header = f"| {' ' * decimal_width} | {' ' * hex_width} | {' ' * binary_width} | {' ' * collatz_width} |\n"
binary_key_header += f"| {' ' * decimal_width} | {' ' * hex_width} | "
while max_bits % binary_split != 0:
    max_bits += 1
segments = []
x = max_bits
while x > 0:
    high = x
    while high % binary_split != 0:
        high += 1
    x -= 1
    while x % binary_split != 0:
        x -= 1
    low = x + 1
    dot_count = max(binary_split - len(str(high)) - len(str(low)), 0)
    segments.append(f"{high}{'.' * dot_count}{low}")
binary_key_header += (' ' * binary_group_spacing).join(segments)
binary_key_header += f" | {' ' * collatz_width} |\n"
binary_key_header += f"| {' ' * decimal_width} | {' ' * hex_width} | "
segments.clear()
for i in range(0, group_count, 1):
    segments.append("=" * binary_split)
binary_key_header += (' ' * binary_group_spacing).join(segments)
binary_key_header += f" | {' ' * collatz_width} |"

# Spit out the table.
print("Table Details")
print(f"Start: {start}, End: {end}, Max Bits: {max_bits}, Decimal Width: {decimal_width}, Hex Width: {hex_width}")
print(separator)
print(f"| {decimal_header} | {hex_header} | {binary_header} | {collatz_header} |")
print(separator)
for i in range(start, end + 1, 1):
    if i % breakup_size == 0:
        print(binary_key_header)
    line = "| "
    line += f"{str(i)[:decimal_width]:>{decimal_width}} | "
    line += f"{str(hex(i))[:hex_width]:>{hex_width}} | "
    binary = format(i, f"0{max_bits}b")
    binary_groups = [binary[i:i+binary_split] for i in range(0, len(binary), binary_split)]
    line += f"{(' ' * binary_group_spacing).join(binary_groups)} | "
    collatz = generate_collatz(i)
    if len(collatz) > collatz_width:
        collatz = collatz[:collatz_width-4] + ' ...'
    line += f"{collatz:<{collatz_width}} |"
    print(line)
print(separator)
