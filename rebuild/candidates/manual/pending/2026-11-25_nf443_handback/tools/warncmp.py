#!/usr/bin/env python3
"""warncmp.py <tag>: per row, vland WARN set HEAD v new (anonymous-namespace hashes normalised); prints rows whose new set has a WARN
kind HEAD did not have, and the RELOC_NAME warns that vanished."""
import sys, re, collections
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF443'
def parse(p):
    d = collections.defaultdict(set)
    for l in open(p, errors='replace'):
        m = re.match(r'^([0-9a-f]{8})\s+WARN (\S+)(.*)$', l.rstrip())
        if m: d[m.group(1)].add((m.group(2).rstrip(':'), re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', m.group(3).strip())))
    return d
t = sys.argv[1]; h, n = parse('%s/work/vland_%sh.log' % (W, t)), parse('%s/work/vland_%sn.log' % (W, t))
newk = 0; gone = 0
for a in sorted(set(h) | set(n)):
    hk = {k for k, _ in h[a]}; add = sorted(k for k, _ in n[a] if k not in hk)
    g = [x for x in h[a] - n[a] if x[0] == 'RELOC_NAME']; gone += len(g)
    if add: newk += 1; print(a, 'NEW WARN KINDS', add)
print('rows with new warn kinds', newk, '| RELOC_NAME warns removed', gone)
