#!/usr/bin/env python3
"""order_cal2.py: retail calibration of 'inlined callee before inliner' on named counted-pointer pairs of ONE T: the inline callee
?ReleaseData@?$CCountedPointer@<T> v its inliners ??1?$CCountedPointer@<T> / ?Reset@?$CCountedPointer@<T> (and the CIVCountedPointer /
CDefPointer equivalents when named), both at known addresses (any tracker status whose single spelling is that PDB name), within 0x800.
Also reports whether the callee's body occurs as a call inside the inliner (then it is a CALL, not inlining, and is listed apart)."""
import json, re, collections, sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426/tools')
from pe import PE
p = PE()
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426'
r = json.load(open(A + '/status/rec.json'))
by = collections.defaultdict(dict)
pat = re.compile(r'^(\?ReleaseData@|\?\?1|\?Reset@|\?Release@)\?\$(CCountedPointer|CIVCountedPointer|CDefPointer)@(.+?)@@@@Q')
for k, e in r.items():
    if e['status'] not in ('agree_pdb', 'agree_std') or e['n_spellings'] != 1: continue
    nm = e['names'][0] if e['names'] else ''
    m = pat.match(nm)
    if m: by[(m.group(2), m.group(3))][m.group(1)] = int(k, 16)
c = collections.Counter(); rows = []
def calls(a, tgt, n=0x200):
    b = p.read(a, n) or b''
    return any(b[i] == 0xE8 and (a + i + 5 + int.from_bytes(b[i + 1:i + 5], 'little', signed=True)) & 0xffffffff == tgt for i in range(len(b) - 5))
for (tm, T), d in sorted(by.items()):
    callee = d.get('?ReleaseData@') if tm == 'CCountedPointer' else d.get('?Release@')
    if not callee: continue
    for inl in ('??1', '?Reset@'):
        y = d.get(inl)
        if not y or abs(y - callee) > 0x800: continue
        kind = 'call' if calls(y, callee) else 'inlined'
        k = (kind, 'callee_lower' if callee < y else 'callee_higher'); c[k] += 1
        rows.append('%s\t%s<%s>\t%s %08x\t%s %08x\t%s' % (k[0], tm, T[:40], 'callee', callee, inl, y, k[1]))
for k, n in sorted(c.items()): print(n, k)
open(A + '/evidence/order_cal2.tsv', 'w').write('\n'.join(rows) + '\n')
