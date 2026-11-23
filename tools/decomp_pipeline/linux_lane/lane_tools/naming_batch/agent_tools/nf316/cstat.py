# cstat.py <idx..>: per ENTRIES line, the self + every census row with its flags: OFF (offlimits), OE (NF308/NF309 entry),
# OW (NF308/NF309 witness), LOCK=<owner> (a lock held by someone else). Use before choosing a group's changed / control rows.
import sys, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); S = os.path.dirname(os.path.dirname(A))
off = set(open(A + '/work/offlimits.txt').read().split()); oe = set(open(A + '/work/other_agents_entries.txt').read().split())
ow = set(open(A + '/work/other_agents_witnesses.txt').read().split())
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')][1:]
C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
def fl(a):
    f = []
    if a in off: f.append('OFF')
    if a in oe: f.append('OE')
    if a in ow: f.append('OW')
    o = S + '/w22/nflocks/%s/owner' % a
    if os.path.exists(o):
        w = open(o).read().strip()
        if w not in ('released', 'NF316'): f.append('LOCK=' + w)
        elif w == 'NF316': f.append('MINE')
    return a + ('[' + ','.join(f) + ']' if f else '')
for i in map(int, sys.argv[1:]):
    a = E[i - 1][0]
    print('#%d self %s  census: %s' % (i, fl(a), ' '.join(fl(x) for x in C.get(i, []))))
