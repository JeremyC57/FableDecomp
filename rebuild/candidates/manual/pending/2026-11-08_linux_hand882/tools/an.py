"""an.py <events.json> [func-substring]: list the plain-inline decisions (c2.dll 0x1078D1AA) of one function:
r (1 = expanded), sz, pen, savings, c1 (cost after savings), f (static site frequency), sc (score; expanded iff sc < T)."""
import json, sys
ev = json.load(open(sys.argv[1]))
want = sys.argv[2] if len(sys.argv) > 2 else '?ConstructQuestListPC@'
pend = []; groups = []
for e in ev:
    if e[0] == 'd': pend.append(e[1])
    elif e[0] == 'pipe': groups.append((e[1], pend)); pend = []
for name, ds in groups:
    if name and want in name:
        print(name, len(ds), 'decisions')
        for i, d in enumerate(ds):
            f = (d['fhi'] * 2**32 + d['flo']) / 1e8
            print(f"{i:4d} r={d['r']} sz={d['sz']:4d} pen={d.get('pen','-')!s:>4} sav={d.get('sav','-')} c0={d.get('c0','-')!s:>4} c1={d.get('c1','-')!s:>4} f={f:.4f} sc={d.get('sc','-')!s:>5} {d['n'][:110]}")
