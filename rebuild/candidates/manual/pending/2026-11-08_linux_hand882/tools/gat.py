"""gat.py <json>: g / a / f / r / sc at each CCountedPointer<CAction>(T*), CAction() and late CCountedPointer<CComponent>(T*) site of the row."""
import json, sys
ev = json.load(open(sys.argv[1]))
pend = []; groups = []
for e in ev:
    if e[0] == 'd': pend.append(e[1])
    elif e[0] == 'pipe': groups.append((e[1], pend)); pend = []
ds = [d for n, d in groups if n and 'ConstructQuestListPC' in n][0]
print('decisions', len(ds), 'g0', ds[0]['g'], 'gend', ds[-1]['g'])
for i, d in enumerate(ds):
    n = d['n']
    if n.startswith('??0?$CCountedPointer@VCAction') or n.startswith('??0CAction@') or (n.startswith('??0?$CCountedPointer@VCComponent') and 'PAV' in n and d['g'] > 7000) or n.startswith('?ConvertVector'):
        f = (d['fhi'] * 2**32 + d['flo']) / 1e8
        print(f"{i:4d} g {d['g']:5d} a+g {d['a']+d['g']:5d} f {f:7.3f} pen {d.get('pen')} sc {d.get('sc')} r {d['r']} {n[:48]}")
