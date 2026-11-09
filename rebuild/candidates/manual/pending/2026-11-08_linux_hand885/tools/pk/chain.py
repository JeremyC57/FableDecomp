#!/usr/bin/env python3
"""chain.py <prio.trace> <line> <id>: follow a split piece back to its root candidate through the c2prio trace
(ids are reused, so each step takes the latest 'piece #id of #p' before the line). For each ancestor prints: line it was
created, the split pass that split it (what was coloured just before = the decision that exhausted its registers), and
its priority in each re-sort while it existed."""
import re, sys
L = open(sys.argv[1]).read().split('\n')
line, cid = int(sys.argv[2]), int(sys.argv[3])
def latest(pat, before):
    for i in range(before - 1, -1, -1):
        if pat.search(L[i]): return i + 1
    return None
chain = []
cur_line, cur = line, cid
while True:
    m = latest(re.compile(r'piece +#%d +of #(\d+)' % cur), cur_line)
    if not m: break
    parent = int(re.search(r'of #(\d+)', L[m-1]).group(1))
    chain.append((cur, m, parent))
    cur_line, cur = m, parent
print('root #%d (as of line %d)' % (cur, cur_line))
for (c, m, p) in reversed(chain):
    # the split pass line just above m, and the last choose before it
    sp = latest(re.compile(r'split pass'), m)
    ch = latest(re.compile(r'^  -> '), sp) if sp else None
    print('line %6d: piece #%d of #%d   split pass @%s after %s' % (m, c, p, sp, L[ch-1].strip()[:90] if ch else '-'))
# prios of each node while it lived
nodes = [(cid, line)] + [(p, m) for (c, m, p) in chain]
for (c, until) in nodes:
    born = next((m for (cc, m, p) in chain if cc == c and m <= until), 0)
    out = []
    for i in range(born, until):
        if 're-sort' in L[i]:
            x = re.search(r'#%d\((-?\d+)\)' % c, L[i])
            if x: out.append('%d:%s' % (i+1, x.group(1)))
    print('#%d [%d..%d] prios: %s' % (c, born, until, ' '.join(out[:12]) + (' ...' if len(out) > 12 else '')))
