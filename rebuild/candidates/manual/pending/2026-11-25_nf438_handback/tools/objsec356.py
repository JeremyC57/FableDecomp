# objsec356.py <A.obj> <B.obj> <leafA> <leafB>: objsec352 (stable section keys, $L labels resolved to (key, value)) plus a leaf RENAME:
# the section that defines <leafA> in A and <leafB> in B is compared as one key ('LEAF'), with relocations naming leafA mapped to leafB.
# Prints the leaf verdict (data / relocs / characteristics / COMDAT selection for each side), every other non-debug section key that
# differs, is added or dropped, the .drectve token delta, and the external DEF / UND symbol deltas. No bytes.
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
def sel_of(p):
    d = open(p, 'rb').read(); psym, nsym = struct.unpack_from('<II', d, 8); sel = {}; i = 0
    while i < nsym:
        o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8)
        if scl == 3 and v == 0 and na == 1 and sec > 0: sel.setdefault(sec, d[o + 18 + 14])
        i += 1 + na
    return sel
def load(p, leaf, ren):
    secs, sl = nf.coff(p); first = {}; sel = sel_of(p)
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    leafsec = [sec for i, nm, v, sec, t, scl in sl if nm == leaf and sec > 0]
    leafsec = leafsec[0] if leafsec else None
    key = {}; cnt = {}; last = None
    for k, x in enumerate(secs, 1):
        if k == leafsec: key[k] = ('LEAF',); last = 'LEAF'
        elif x['ch'] & 0x1000 and k in first: key[k] = ('COMDAT', first[k], x['name']); last = first[k]
        elif x['ch'] & 0x1000 and last: key[k] = ('ASSOC', last, x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key[k] = ('SEC', x['name'], cnt[x['name']])
    lab = {nm: (key.get(sec), v) for i, nm, v, sec, t, scl in sl if nm.startswith('$L')}
    out = {}
    for k, x in enumerate(secs, 1):
        if x['name'].startswith('.debug'): continue
        kk = key[k]
        while kk in out: kk = kk + ('dup',)
        out[kk] = (x['data'], [(o, ren.get(lab.get(s, s), lab.get(s, s)), t) for o, s, t in x['rel']], x['ch'])
    info = None
    if leafsec: x = secs[leafsec - 1]; info = dict(name=x['name'], ch='%08x' % x['ch'], sel=sel.get(leafsec), size=len(x['data']))
    syms = {(s[:4] + ren.get(s[4:], s[4:])) for s in nf.ext_syms(p)}
    drect = [s['data'].decode('latin1').split() for s in secs if s['name'] == '.drectve']
    return out, syms, info, (drect[0] if drect else [])
la, lb = sys.argv[3], sys.argv[4]
a, sa, ia, da = load(sys.argv[1], la, {la: lb}); b, sb, ib, db = load(sys.argv[2], lb, {})
print('LEAF A', la[:100], ia); print('LEAF B', lb[:100], ib)
n = 0; lv = 'absent'
for k in sorted(set(a) | set(b), key=str):
    if k == ('LEAF',):
        if k in a and k in b:
            x, y = a[k], b[k]; lv = 'data_eq=%s relocs_eq=%s ch_eq=%s' % (x[0] == y[0], x[1] == y[1], x[2] == y[2])
        continue
    if k not in b: print('DROPPED', k, 'size=%d' % len(a[k][0])); n += 1; continue
    if k not in a: print('ADDED  ', k, 'size=%d' % len(b[k][0])); n += 1; continue
    x, y = a[k], b[k]
    if x != y:
        print('CHANGED', k, 'size %d -> %d' % (len(x[0]), len(y[0])), 'data_eq=%s relocs_eq=%s flags_eq=%s' % (x[0] == y[0], x[1] == y[1], x[2] == y[2])); n += 1
print('DRECTVE -', sorted(set(da) - set(db)), '+', sorted(set(db) - set(da)))
for s in sorted(sa - sb): print('SYM-', s)
for s in sorted(sb - sa): print('SYM+', s)
print('LEAFCMP', lv)
print('VERDICT', 'OTHER_SAME' if n == 0 else 'OTHER_DIFF n=%d' % n, 'symdelta=%d' % len(sa ^ sb))
