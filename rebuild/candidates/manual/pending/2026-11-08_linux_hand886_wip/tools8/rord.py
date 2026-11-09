"""rord.py <tag>: our packer slots with densities, in RETAIL's final order (by retail offset from fmap), flagging
density-order violations (retail must have a different weight/size there)."""
import json, re, sys
D = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr/'
tag = sys.argv[1]
ev = json.load(open(D + tag + '.json'))['events']
pe = [e for e in ev if e[0] == 'packend'][0]
pk = [e for e in ev if e[0] == 'pack'][0]
L = pe[3]; slots = pe[4]; idx = pe[5]
byid = {x[0]: x for x in L}
fm = {}
for line in open(D + tag + '.fmap'):
    m = re.match(r'ours\s+(-?0x[0-9a-f]+) -> retail\s+(-?0x[0-9a-f]+)\s+delta\s+([+-]0x[0-9a-f]+)\s+n=(\d+)/(\d+)', line)
    if m: fm[int(m.group(1), 16)] = (int(m.group(2), 16), int(m.group(4)), int(m.group(5)))
def dens(w, s): return (abs(w * 1000) // s) * (1 if w >= 0 else -1)
rows = []
for s in slots:
    i, mem, size, w, off, al, flag = s
    names = [byid[idx[k]][1] or '?' for k in (mem or [])]
    offs = set(byid[idx[k]][10] for k in (mem or []))
    o = min(offs) if offs else None
    ret = None; votes = None
    if o is not None:
        for d in range(0, max(size, 4)):
            if o + d in fm:
                r = fm[o + d]; ret = r[0] - d; votes = r[1:]; break
    rows.append(dict(i=i, off=o, size=size, w=w, d=dens(w, size), flag=flag & 1, ret=ret, votes=votes, names=names))
if __name__ == '__main__':
    have = [r for r in rows if r['ret'] is not None and r['i'] >= 1]
    print('slots', len(rows), 'paired', len(have), 'same offset', sum(1 for r in have if r['ret'] == r['off']))
    have.sort(key=lambda r: -r['ret'])
    prev = None
    for r in have:
        bad = prev is not None and (r['flag'], r['d']) > (prev['flag'], prev['d']) and r['flag'] == prev['flag']
        print(f"{'!!' if bad else '  '} ret {r['ret']:6d} ours {r['off']:6d} i {r['i']:3d} sz {r['size']:3d} w {r['w']:4d} d {r['d']:6d} f {r['flag']} v {r['votes']} " + ','.join(r['names'])[:80])
        prev = r
