# aflags.py <addr..>: OFF / OE / OW / LOCK=<owner> / MINE flags for arbitrary rows (cstat.py for non-ENTRIES addresses)
import sys, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); S = os.path.dirname(os.path.dirname(A))
off = set(open(A + '/work/offlimits.txt').read().split()); oe = set(open(A + '/work/other_agents_entries.txt').read().split())
ow = set(open(A + '/work/other_agents_witnesses.txt').read().split())
out = []
for a in sys.argv[1:]:
    a = a.lower().replace('0x', ''); f = []
    if a in off: f.append('OFF')
    if a in oe: f.append('OE')
    if a in ow: f.append('OW')
    o = S + '/w22/nflocks/%s/owner' % a
    if os.path.exists(o):
        w = open(o).read().strip()
        if w not in ('released', 'NF312'): f.append('LOCK=' + w)
        elif w == 'NF312': f.append('MINE')
    out.append(a + ('[' + ','.join(f) + ']' if f else ''))
print(' '.join(out))
