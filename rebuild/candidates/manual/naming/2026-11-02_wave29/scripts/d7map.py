# d7map.py: for each D7 row (wave 28 sfb / esb own rows) -- the vtable whose slot 0 is the row, its RTTI (if any), the code storing
# the vtable (catalog owner), the bound ??_7 spelling, and the callees of the retail body (call targets with their bound spellings).
import sys, os, struct, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pe
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIND = collections.defaultdict(set)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 2 and q[0] != 'symbol': BIND[q[1].lower()].add(q[0])
CAT = {}
for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv'):
    s = l.rstrip('\n').split('\t')
    if len(s) >= 4 and s[0] != 'address': CAT[s[0].lower()] = (s[1], int(s[2]), s[3])
def rtti(vt):
    col = pe.dw(vt - 4)
    try:
        if pe.dw(col) != 0: return None
        td = pe.dw(col + 12); return pe.cstr(td + 8)
    except Exception: return None
def callees(a):
    nm, n, hx = CAT[a]; b = bytes.fromhex(hx); out = []
    base = int(a, 16)
    for i in range(len(b) - 4):
        if b[i] in (0xE8, 0xE9):
            t = (base + i + 5 + struct.unpack_from('<i', b, i + 1)[0]) & 0xffffffff
            if pe.owner(t) or t in (0x00BFE9BC,): out.append((i, '%08x' % t))
    return out
fams = sys.argv[1:]
for fam in fams:
    for a in [l.split()[0] for l in open(os.path.join(W, 'runs', 'base_w28_%s.rows.txt' % fam))]:
        refs = [va for nm, va in pe.find(int(a, 16), ('.rdata',))]
        vts = []
        for va in refs:
            st = [r for r in pe.find(va, ('.text',))]
            vts.append(('%08x' % va, rtti(va), sorted(set((pe.owner(r) or '?%08x' % r).split()[0] for _, r in st)), sorted(BIND.get('%08x' % va, ()))))
        cl = callees(a)
        print('\t'.join([fam, a, CAT[a][0], str(CAT[a][1]), repr(vts), ' '.join('%d:%s=%s' % (i, t, '|'.join(sorted(BIND.get(t, ()))[:2])) for i, t in cl)]))
