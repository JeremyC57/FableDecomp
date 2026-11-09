"""fit.py: retail slot offsets per node (via fmap pairing of OUR real slots) + fitness of a simulated layout."""
import json, re, sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr')
import pack
D = pack.D
def fmap(tag):
    fm = {}
    for line in open(D + tag + '.fmap'):
        m = re.match(r'ours\s+(-?0x[0-9a-f]+) -> retail\s+(-?0x[0-9a-f]+)\s+delta\s+([+-]0x[0-9a-f]+)\s+n=(\d+)/(\d+)', line)
        if m: fm[int(m.group(1), 16)] = (int(m.group(2), 16), int(m.group(4)), int(m.group(5)))
    return fm
def slot_bases(M, slots, B):
    """node -> slot base offset for a (sorted) slot list, using the layout rule."""
    cur = -4; res = {}; bases = []
    for k, s in enumerate(slots):
        if k < B: off = 124
        else:
            sz = s['size']; al = 1 if sz == 1 else (2 if sz == 2 else 4)
            cur = -(((-cur) + sz + al - 1) // al * al); off = cur + 0x74
        bases.append(off)
        for i in s['mem']: res[M['idx2nid'][i]] = off
    return res, bases
def retail_targets(M, tag):
    """node -> retail slot base (from our REAL layout + fmap)."""
    pe = [e for e in M['ev'] if e[0] == 'packend'][0]
    real = [dict(mem=set(s[1] or []), size=s[2]) for s in pe[4]]
    B = pe[2]
    ours, bases = slot_bases(M, real, B)
    fm = fmap(tag)
    tgt = {}
    for s, base in zip(real, bases):
        r = None
        for d in range(0, max(s['size'], 4)):
            if base + d in fm: r = fm[base + d][0] - d; break
        for i in s['mem']:
            tgt[M['idx2nid'][i]] = r
    return tgt, ours
def fitness(M, tgt, slots, B):
    res, _ = slot_bases(M, slots, B)
    good = sum(1 for nid, r in tgt.items() if r is not None and res.get(nid) == r)
    tot = sum(1 for r in tgt.values() if r is not None)
    return good, tot
if __name__ == '__main__':
    tag = sys.argv[1]
    M = pack.load(tag)
    tgt, ours = retail_targets(M, tag)
    pre, slots, B, total = pack.pack(M)
    print('fitness (nodes at retail slot base):', fitness(M, tgt, slots, B))
    # slots view
    res, bases = slot_bases(M, slots, B)
    nm = lambda i: M['nodes'][M['idx2nid'][i]]['name'] or '?'
    for k, (s, b) in enumerate(zip(slots, bases)):
        rs = sorted(set(tgt[M['idx2nid'][i]] for i in s['mem']), key=lambda x: (x is None, x))
        d = pack.qs  # noqa
        dens = (abs(s['w'] * 1000) // s['size']) if s['size'] else 0
        print(f"{k:4d} base {b:6d} ret {rs} sz {s['size']:3d} w {s['w']:4d} d {dens:6d} f {s['flag']} " + ','.join(nm(i) for i in sorted(s['mem']))[:70])
