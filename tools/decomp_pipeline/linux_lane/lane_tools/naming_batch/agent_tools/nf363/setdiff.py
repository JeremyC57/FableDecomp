# setdiff.py <head.obj> <new.obj> [old=new ...]: the COMDAT-set change of a row, HEAD v new, keyed by (section name, defined symbol):
# DROPPED / ADDED sections (with selection, size), CHANGED (selection, size, alignment, bytes, relocation symbols under the renames),
# .drectve token delta, DEF / UND deltas. Names and sizes only, no bytes (NF359).
import sys, struct
def load(p):
    d = open(p, 'rb').read(); ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8); oh = struct.unpack_from('<H', d, 16)[0]
    strt = d[psym + 18 * nsym:]
    def nm(raw):
        if raw[:4] == b'\0\0\0\0': o = struct.unpack_from('<I', raw, 4)[0]; return strt[o:strt.index(b'\0', o)].decode('latin1')
        return raw.rstrip(b'\0').decode('latin1')
    syms = {}; sel = {}; ext = {}; stat = {}; und = set(); dfn = set(); i = 0
    while i < nsym:
        o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8); n = nm(d[o:o + 8]); syms[i] = n
        if scl == 3 and v == 0 and na == 1 and sec > 0 and sec not in sel: sel[sec] = d[o + 18 + 14]
        if scl == 2 and sec > 0: ext.setdefault(sec, n); dfn.add(n)
        if scl == 2 and sec == 0: und.add(n)
        if sec > 0 and scl == 3 and not n.startswith('.') and not n.startswith('$'): stat.setdefault(sec, n)
        i += 1 + na
    out = {}
    for k in range(ns):
        h = d[20 + oh + 40 * k:20 + oh + 40 * k + 40]; name = h[:8].rstrip(b'\0').decode('latin1')
        if name.startswith('/'): off = int(name[1:]); name = strt[off:strt.index(b'\0', off)].decode('latin1')
        if name.startswith('.debug'): continue
        sz, ptr, rp = struct.unpack_from('<III', h, 16); nr = struct.unpack_from('<H', h, 32)[0]; ch = struct.unpack_from('<I', h, 36)[0]
        data = d[ptr:ptr + sz] if ptr else b''
        rel = [(struct.unpack_from('<I', d, rp + 10 * j)[0], syms.get(struct.unpack_from('<I', d, rp + 10 * j + 4)[0]), struct.unpack_from('<H', d, rp + 10 * j + 8)[0]) for j in range(nr)]
        key = (name, ext.get(k + 1) or ('static:' + stat.get(k + 1, '-')))
        if key in out: key = (name, key[1] + '#%d' % k)
        out[key] = dict(sel=sel.get(k + 1, '-'), size=sz, ch=ch, data=data, rel=rel)
    return out, und, dfn
H, uh, dh = load(sys.argv[1]); N, un, dn = load(sys.argv[2]); mp = dict(x.split('=', 1) for x in sys.argv[3:])
def r(k): return (k[0], mp.get(k[1], k[1]))
Hm = {r(k): v for k, v in H.items()}
for k in sorted(set(Hm) - set(N)): print('DROPPED  %-8s sel=%s size=%d %s' % (k[0], Hm[k]['sel'], Hm[k]['size'], k[1]))
for k in sorted(set(N) - set(Hm)): print('ADDED    %-8s sel=%s size=%d %s' % (k[0], N[k]['sel'], N[k]['size'], k[1]))
for k in sorted(set(N) & set(Hm)):
    a, b = Hm[k], N[k]; diffs = []
    if a['sel'] != b['sel']: diffs.append('selection %s->%s' % (a['sel'], b['sel']))
    if a['size'] != b['size']: diffs.append('size %d->%d' % (a['size'], b['size']))
    if a['ch'] != b['ch']: diffs.append('characteristics %08x->%08x' % (a['ch'], b['ch']))
    if k[0] == '.drectve':
        ta, tb = a['data'].decode('latin1').split(), b['data'].decode('latin1').split()
        if ta != tb: diffs.append('tokens HEAD-only=%s new-only=%s' % (sorted(set(ta) - set(tb)), sorted(set(tb) - set(ta))))
    elif a['data'] != b['data']: diffs.append('bytes differ')
    ra = [(o, mp.get(s, s), t) for o, s, t in a['rel']]
    if ra != b['rel'] and k[0] != '.drectve': diffs.append('relocs %s -> %s' % (sorted(set(s for _, s, _ in ra) - set(s for _, s, _ in b['rel']))[:6], sorted(set(s for _, s, _ in b['rel']) - set(s for _, s, _ in ra))[:6]))
    print('%s %-8s %s %s' % ('CHANGED ' if diffs else 'SAME    ', k[0], k[1], '; '.join(diffs)))
uhm = {mp.get(x, x) for x in uh}; dhm = {mp.get(x, x) for x in dh}
print('DEF dropped:', sorted(dhm - dn)); print('DEF added:', sorted(dn - dhm))
print('UND dropped:', sorted(uhm - un)); print('UND added (%d):' % len(un - uhm), sorted(un - uhm))
