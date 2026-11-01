# mergefix.py <tag>: fold a single-row fix recheck (runs/rk_<tag>.*) and its index (idx/<tag>.jsonl) into the final artifacts.
import json, sys, re
tag = sys.argv[1]
fx = {json.loads(l)['a']: l for l in open('runs/rk_%s.pairs.jsonl' % tag)}
rows = set(fx)
L = []
for l in open('runs/rk_final.pairs.jsonl'):
    a = json.loads(l)['a']; L.append(fx.pop(a, l))
L += list(fx.values()); open('runs/rk_final.pairs.jsonl', 'w').write(''.join(L))
fl = [l for l in open('runs/rk_%s.log' % tag) if re.match(r'^[0-9a-f]{8} ', l)]
out = [l for l in open('runs/rk_final.log') if l[:8] not in rows] + fl
open('runs/rk_final.log', 'w').writelines(out)
ix = {json.loads(l)['a']: l for l in open('idx/%s.jsonl' % tag)}
for fn in ['idx/ovfinal.jsonl', 'idx/after.jsonl']:
    seen = set(); L = []
    for l in open(fn):
        a = json.loads(l)['a']; seen.add(a); L.append(ix.get(a, l))
    if fn == 'idx/ovfinal.jsonl': L += [v for a, v in ix.items() if a not in seen]
    open(fn, 'w').write(''.join(L)); print(fn, len(L))
fr = set(l.strip() for l in open('runs/final.rows.txt') if l.strip()) | rows
open('runs/final.rows.txt', 'w').write(''.join(a + '\n' for a in sorted(fr))); print('rows', len(fr))
