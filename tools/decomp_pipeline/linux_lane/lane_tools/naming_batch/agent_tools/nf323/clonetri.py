# clonetri.py <Class...>: per class, the retail vftable (rebuild/manifest/rtti-vtables.tsv), its slot count (targets in .text), and
# every slot whose RTTI-port label is NOT a base-chain member (an override in retail), with whether engine/<Class>.h mentions the
# member name. No bytes printed. (NF323, 2026-11-23)
import sys, struct, os, re
R = '/home/user/FableDecomp'
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; S_ = []
for i in range(ns):
    nm = d[so + i * 40: so + i * 40 + 8].rstrip(b'\0').decode(); vsz, vaddr = struct.unpack_from('<II', d, so + i * 40 + 8); raw = struct.unpack_from('<I', d, so + i * 40 + 20)[0]
    S_.append((nm, base + vaddr, vsz, raw))
def rd(x):
    for nm, s0, n0, r in S_:
        if s0 <= x < s0 + n0: return struct.unpack_from('<I', d, r + x - s0)[0]
def intext(x): return any(nm == '.text' and s0 <= x < s0 + n0 for nm, s0, n0, r in S_)
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open(R + '/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
VT = {}
for l in open(R + '/rebuild/manifest/rtti-vtables.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 4: VT.setdefault(p[0], set()).add(p[1])
BASES = {}
for l in open(R + '/ghidra_out/rtti_bases.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 5: BASES[p[1]] = [b.split('@')[0] for b in p[4].split(';') if b]
for c in sys.argv[1:]:
    vts = sorted(VT.get(c, []))
    chain = BASES.get(c, [])
    h = R + '/rebuild/include/engine/' + c + '.h'
    ht = open(h).read() if os.path.exists(h) else ''
    for v in vts:
        va = int(v, 16); k = 0; ov = []
        while True:
            t = rd(va + 4 * k)
            if t is None or not intext(t): break
            lab = L.get('%08x' % t, '-')
            m = re.match(r'\?\?_[GE](\w+)@@|\?(\w+)@(\w+)@@', lab)
            cls = (m.group(1) or m.group(3)) if m else '?'
            name = m.group(2) if m and m.group(2) else ('dtor' if m else '?')
            if cls not in chain:
                inh = 'n/a' if name == 'dtor' else ('HDR' if re.search(r'\b%s\s*\(' % re.escape(name), ht) else 'MISSING')
                ov.append('%d:%s:%s:%s' % (k, name, cls if cls != c else '=', inh))
            k += 1
        print('%s\tvt=%s\tslots=%d\thdr=%s\t%s' % (c, v, k, 'Y' if ht else 'N', ' '.join(ov)))
