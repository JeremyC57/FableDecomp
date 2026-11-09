"""slotcmp.py <tag>: our packer slots (creation order) v retail's offsets (fmap pairing) -> retail creation order."""
import json, re, sys, collections
tag = sys.argv[1]; D = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr/'
ev = json.load(open(D + tag + '.json'))['events']
pe = [e for e in ev if e[0] == 'packend'][0]
L = pe[3]; slots = pe[4]; idx = pe[5]
byid = {x[0]: x for x in L}
nodes_by_index = idx  # node index -> nid
fm = {}
for line in open(D + tag + '.fmap'):
    m = re.match(r'ours\s+(-?0x[0-9a-f]+) -> retail\s+(-?0x[0-9a-f]+)\s+delta\s+([+-]0x[0-9a-f]+)\s+n=(\d+)/(\d+)', line)
    if m: fm[int(m.group(1), 16)] = (int(m.group(2), 16), int(m.group(4)), int(m.group(5)))
rows = []
for s in slots:
    i, mem, size, w, off, al, x = s
    names = [byid[idx[k]][1] for k in (mem or [])]
    lo = off + 0x74 if False else None
    # our listing offset of the slot: any member's final sym offset
    offs = set(byid[idx[k]][10] for k in (mem or []))
    o = min(offs) if offs else None
    ret = None
    if o is not None:
        for d in range(0, max(size, 4)):
            if o + d in fm:
                r = fm[o + d]; ret = r[0] - d; break
    rows.append((i, o, size, w, ret, names))
mode = sys.argv[2] if len(sys.argv) > 2 else 'list'
if mode == 'list':
    for r in rows:
        print(f'{r[0]:4d} ours {r[1]!s:>6} sz {r[2]:3d} w {r[3]:4d} retail {r[4]!s:>6} {"=" if r[1]==r[4] else " "} ' + ','.join(n or '?' for n in r[5])[:110])
