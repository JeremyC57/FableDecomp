#!/usr/bin/env python3
"""kgain.py <c2prio.trace> <A> <B>: for every other candidate C, how much prio(A)-prio(B) changes if C stopped being counted in
K (removed from every block that references it): per block, a referenced candidate loses its cost, a live-through one gains 1.
Negative = removing C helps B overtake A. Also the table name/lines of C."""
import sys, re, collections
tr, A, B = sys.argv[1], sys.argv[2], sys.argv[3]
blocks = []; cur = None; names = {}
for line in open(tr, errors='replace'):
    m = re.match(r'\s+\d+\s+(\d+)\s+(\S+)\s+(\S*)\s+(-?\d+)\s+(\d+)\s+(-?\d+)\s+(\d+)\s', line)
    if m and not blocks: names.setdefault(m.group(1), f'{m.group(2)} {m.group(3)} prio {m.group(4)} refs {m.group(7)}')
    m = re.match(r'\s+block (\d+) .*w (\d+) K (\d+)', line)
    if m: cur = {'n': int(m.group(1)), 'K': int(m.group(3)), 'r': {}, 'l': set()}; blocks.append(cur); continue
    m = re.match(r'\s+#(\d+)\s+(.+?)\s+(referenced, cost\s+(\d+)|live through, no reference): ([+-]\d+)', line)
    if m and cur is not None:
        if m.group(4): cur['r'][m.group(1)] = int(m.group(4)); names.setdefault(m.group(1), m.group(2))
        else: cur['l'].add(m.group(1))
def term(b, x):
    if x in b['r']: return b['r'][x]
    if x in b['l']: return -1
    return 0
gain = collections.Counter(); nb = collections.Counter()
for b in blocks:
    for c in b['r']:
        if c in (A, B): continue
        gain[c] += -term(b, A) + term(b, B); nb[c] += 1
for c, v in sorted(gain.items(), key=lambda kv: kv[1])[:int(sys.argv[4]) if len(sys.argv) > 4 else 40]:
    print(f'#{c:5s} {names.get(c, "?"):50s} blocks {nb[c]:3d}  gap change if removed: {v:+d}')
