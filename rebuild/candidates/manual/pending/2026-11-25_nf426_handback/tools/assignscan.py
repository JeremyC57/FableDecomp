#!/usr/bin/env python3
"""assignscan.py: for each accepted single-T TU (lower copy L, higher copy H), find the CDefPointer assign bodies in the TU by their tails
(`inc [eax+4]; pop edi; mov eax,esi; pop esi; ret 4` = operator= returning *this; `inc [eax+4]; pop edi; pop esi; ret 4` = the void
SetDynamicCast) between the TU's plain anchors, and place each one: below L, between L and H, directly after H (first body after it),
or later. Under 'an inline's copy precedes every COMDAT that expands it', an assign of this T must not sit below L (Release)."""
import sys, os, re, json, bisect, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); from pe import PE
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426'
p = PE(); occ = json.load(open(A + '/evidence/occ.json')); t = json.load(open(A + '/evidence/occ_tu.json'))
S = [x for x in json.load(open(A + '/evidence/single.json')) if x['nego'] and all(x['fam'])]
TAIL = {'op=': bytes.fromhex('ff40045f8bc65ec20400'), 'SDC': bytes.fromhex('ff40045f5ec20400')}
c = collections.Counter(); out = []
for x in S:
    L, H = sorted(int(a, 16) for a in x['addrs'])
    lo = int(t['%08x' % L][1], 16); hi = int(t['%08x' % H][3], 16)
    d = p.read(lo, hi - lo); hits = []
    for k, tl in TAIL.items():
        for m in re.finditer(re.escape(tl), d): hits.append((lo + m.end(), k))
    hits.sort(); n = 27 if occ['%08x' % L][0] == 'T' else 24
    pos = []
    for end, k in hits:
        if end <= L: w = 'below_L'
        elif end <= H: w = 'between'
        else: w = 'after_H'
        pos.append('%s@%08x:%s' % (k, end, w)); c[(k, w)] += 1
    # the body right after H: does it end with an op= tail within 0x60 bytes?
    nxt = p.read(H + n, 0x60); f = [k for k, tl in TAIL.items() if tl in nxt]
    first_after_H = min(((nxt.find(TAIL[k]), k) for k in f), default=(None, '-'))[1]
    nxtL = p.read(L + n, 0x60); fL = [k for k, tl in TAIL.items() if tl in nxtL]
    first_after_L = min(((nxtL.find(TAIL[k]), k) for k in fL), default=(None, '-'))[1]
    c[('next_after_H', first_after_H)] += 1; c[('next_after_L', first_after_L)] += 1
    out.append('%s\tL=%08x\tH=%08x\tnext_after_L=%s\tnext_after_H=%s\t%s' % (x['obj'].split('\\')[-1], L, H, first_after_L, first_after_H, ' '.join(pos) or '-'))
open(A + '/evidence/assignscan.tsv', 'w').write('\n'.join(out) + '\n')
for k, v in sorted(c.items()): print(v, k)
