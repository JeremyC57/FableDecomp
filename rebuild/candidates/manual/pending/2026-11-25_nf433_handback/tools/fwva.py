#!/usr/bin/env python3
"""fwva.py <va...>: FableWin.pdb public names (any kind) at each FableWin VA (exact, else the nearest below with +offset)."""
import re, struct, sys, bisect
E = '/tmp/claude-0/egod_pdb/FableWin.exe'; P = '/tmp/claude-0/egod_pdb/FableWin.publics.txt'
d = open(E, 'rb').read(); pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]
op = struct.unpack_from('<H', d, pe + 20)[0]; ib = struct.unpack_from('<I', d, pe + 24 + 28)[0]
secs = [struct.unpack_from('<IIII', d, pe + 24 + op + 40 * i + 8) for i in range(ns)]
name = None; pubs = []
for l in open(P, encoding='utf-8', errors='replace'):
    m = re.search(r'S_PUB32 \[size = \d+\] `(.*)`', l)
    if m: name = m.group(1); continue
    m = re.search(r'addr = (\d+):(\d+)', l)
    if m and name:
        sec, off = int(m.group(1)), int(m.group(2))
        if 0 < sec <= len(secs): pubs.append((ib + secs[sec - 1][1] + off, name))
        name = None
pubs.sort(); vas = [p[0] for p in pubs]
for x in sys.argv[1:]:
    v = int(x, 16); i = bisect.bisect_right(vas, v) - 1
    print(x, ' | '.join(n for a, n in pubs if a == v) or ('%s+0x%x' % (pubs[i][1], v - pubs[i][0])))
