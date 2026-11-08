#!/usr/bin/env python3
"""h1.py <bigseq.txt>: one line per masked hunk (address, ann line, retail/ours instructions)."""
import re, sys
t=open(sys.argv[1]).read()
for h in t.split('\n@@ ')[1:]:
    lines=h.split('\n')
    m=re.match(r'(\w+) retail\[(\d+):(\d+)\] ann (\S+) (\S+)',lines[0])
    if not m: continue
    tag,i1,i2,ann,ra=m.groups()
    mi=[l[3:].strip() for l in lines[1:] if l.startswith('  - ')]
    pl=[l[3:].strip() for l in lines[1:] if l.startswith('  + ')]
    def s(x): return '; '.join(re.sub(r'^\S+ ','',y) for y in x)[:80]
    print(f'{ra} a{ann:>6} -{int(i2)-int(i1)} +{len(pl)}  R[{s(mi)}]  O[{s(pl)}]')
