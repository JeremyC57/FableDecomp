#!/usr/bin/env python3
"""pb.py <c2prio.trace> <id> [<id>...]: per-block priority terms of candidates (from c2prio --blocks): per id the total of
referenced (+) and live-through (-) terms, the blocks giving the largest terms, and the block numbers where it is referenced."""
import sys, re, collections
tr = sys.argv[1]; ids = sys.argv[2:]
blk = None; K = {}; terms = collections.defaultdict(list)
for line in open(tr, errors='replace'):
    m = re.match(r'\s+block (\d+) .*w (\d+) K (\d+)', line)
    if m: blk = int(m.group(1)); K[blk] = int(m.group(3)); continue
    m = re.match(r'\s+#(\d+)\s+(.+?)\s+(referenced, cost\s+(\d+)|live through, no reference): ([+-]\d+)', line)
    if m and blk is not None and m.group(1) in ids:
        terms[m.group(1)].append((blk, 'r' if m.group(4) else 'l', int(m.group(4) or 0), int(m.group(5))))
for i in ids:
    t = terms[i]; pos = sum(v for b, k, c, v in t if v > 0); neg = sum(v for b, k, c, v in t if v < 0)
    nr = sum(1 for x in t if x[1] == 'r'); nl = sum(1 for x in t if x[1] == 'l')
    print(f'#{i}: total {pos+neg} = +{pos} ({nr} ref blocks) {neg} ({nl} live-through blocks)')
    kh = collections.Counter(K[b] for b, k, c, v in t if k == 'r'); print('   ref-block K histogram', sorted(kh.items()))
    ch = collections.Counter(c for b, k, c, v in t if k == 'r'); print('   ref-block cost histogram', sorted(ch.items()))
    print('   top +', sorted([x for x in t if x[3] > 0], key=lambda x: -x[3])[:12])
    print('   top -', sorted([x for x in t if x[3] < 0], key=lambda x: x[3])[:8])
