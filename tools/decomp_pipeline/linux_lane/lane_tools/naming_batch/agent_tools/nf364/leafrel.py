# leafrel.py <objA> <objB> <leaf symbol>: compare the leaf section of two objects with every relocation target resolved to a STABLE key:
# an external symbol by name; a $L<n> label by (its section's stable key, value), the section key = the first external symbol a COMDAT
# section defines, else (name, ordinal among same-named non-COMDAT sections). objsec346 keys $L labels by raw section INDEX, which shifts
# when a helper COMDAT is dropped. Prints data-equal, reloc-equal and every differing reloc (no bytes).
import sys
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
def load(p):
    secs, sl = nf.coff(p); first = {}
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and sec not in first: first[sec] = nm
    key = {}; cnt = {}
    for k, x in enumerate(secs, 1):
        if x['ch'] & 0x1000 and k in first: key[k] = ('COMDAT', first[k], x['name'])
        else: cnt[x['name']] = cnt.get(x['name'], 0) + 1; key[k] = ('SEC', x['name'], cnt[x['name']])
    lab = {nm: (key.get(sec), v) for i, nm, v, sec, t, scl in sl if nm.startswith('$L')}
    return secs, sl, lab
def leaf(p, sym):
    secs, sl, lab = load(p)
    for i, nm, v, sec, t, scl in sl:
        if nm == sym and sec > 0:
            x = secs[sec - 1]
            return x['data'], [(o, lab.get(s, s), t) for o, s, t in x['rel']]
da, ra = leaf(sys.argv[1], sys.argv[3]); db, rb = leaf(sys.argv[2], sys.argv[3])
print('data_equal', da == db, 'relocs_equal', ra == rb, 'n', len(ra), len(rb))
for x, y in zip(ra, rb):
    if x != y: print('  ', hex(x[0]), str(x[1])[:100], '->', str(y[1])[:100])
