# vtcmp.py <obj> <vftable-symbol> <retail-vtable-va>: the object's emitted vftable slot symbols v the retail slot addresses' RTTI-port labels (NF355)
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
obj, vsym, va = sys.argv[1], sys.argv[2], int(sys.argv[3].replace('0x', ''), 16)
secs, sl = nf.coff(obj)
idx = [sc for (i, n, v, sc, t, c) in sl if n == vsym]
if not idx: print('NO', vsym); sys.exit(1)
s = secs[idx[0] - 1]; off = [v for (i, n, v, sc, t, c) in sl if n == vsym][0]
rel = sorted((o, n) for o, n, t in s['rel'])
lab = {}
R = '/home/user/FableDecomp/'
for f in ('ghidra_out/labels_rtti_port.tsv', 'ghidra_out/labels_rtti_port_dtors.tsv'):
    for l in open(R + f, encoding='utf-8', errors='replace'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 2: lab.setdefault(p[0].lower(), p[1])
ex = nf.exe()
def rd(a):
    for nm, base, d, ch in ex:
        if base <= a < base + len(d) - 3: return struct.unpack_from('<I', d, a - base)[0]
bad = 0
for o, n in rel:
    k = (o - off) // 4
    r = rd(va + 4 * k); rl = lab.get('%08x' % r, '-')
    ok = (n == rl)
    bad += not ok
    print('%2d %-6s obj=%s retail=%08x %s' % (k, 'EQ' if ok else 'DIFF', n, r, rl))
print('SLOTS', len(rel), 'DIFF', bad)
