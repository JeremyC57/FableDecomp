#!/usr/bin/env python3
"""order_cal.py: measure retail COMDAT order. For every reloc_bindings line (callee symbol, callee address, witness = caller row address)
where BOTH caller and callee are template / inline-member COMDATs (caller's own self symbol and the callee symbol contain '?$' or start
with '??$'), within 0x1000 bytes of each other and in the same FableWin.pdb module by the plain-anchor TU map (both sides), count callee
below caller (bottom-up) v above. Prints the rate overall and for CDefPointer / CIVCountedPointer callees."""
import json, bisect, collections, sys, csv
R = '/home/user/FableDecomp'; A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426'
r = json.load(open(A + '/status/rec.json'))
anch = {int(k, 16): v for k, v in json.load(open(A + '/evidence/anchors.json')).items()}
plain = sorted(a for a in anch if '?$' not in r['%08x' % a]['pdb_spelling'] and not r['%08x' % a]['pdb_spelling'].startswith('??_'))
def tu(a):
    i = bisect.bisect(plain, a)
    if i == 0 or i >= len(plain): return None
    lo, hi = anch[plain[i - 1]], anch[plain[i]]
    return lo if lo == hi else None
selfsym = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv'):
    f = l.rstrip('\n').split('\t')
    if len(f) > 1: selfsym[f[0].lower()] = f[1]
tmpl = lambda s: ('?$' in s) or s.startswith('??$')
c = collections.Counter(); cd = collections.Counter(); ex = []
seen = set()
for l in list(open(R + '/rebuild/oracles/reloc_bindings.tsv'))[1:]:
    f = l.rstrip('\n').split('\t')
    if len(f) < 3: continue
    sym, tgt, wit = f[0], f[1].lower(), f[2].lower()
    cs = selfsym.get(wit, '')
    if not (tmpl(sym) and tmpl(cs)) or tgt == wit or (tgt, wit) in seen: continue
    seen.add((tgt, wit))
    t, w = int(tgt, 16), int(wit, 16)
    if abs(t - w) > 0x1000: continue
    ot, ow = tu(t), tu(w)
    if not ot or ot != ow: continue
    k = 'below' if t < w else 'above'; c[k] += 1
    if 'CDefPointer' in sym or 'CIVCountedPointer' in sym: cd[k] += 1; ex.append((k, tgt, wit, sym[:60], cs[:60]))
print('template callee v template caller, same TU, <0x1000 apart:', dict(c), 'bottom-up rate %.1f%%' % (100.0 * c['below'] / max(1, sum(c.values()))))
print('CDefPointer / CIVCountedPointer callees:', dict(cd))
for e in ex[:20]: print(' ', *e)
