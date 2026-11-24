# vtmap.py <vtable-va> [nslots]: retail vftable slots -> address, RTTI-port label, reloc_bindings symbols (names only; no bytes kept) (NF355)
import sys, os, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp/'
va = int(sys.argv[1].replace('0x', ''), 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 64
lab = {}
for f in ('ghidra_out/labels_rtti_port.tsv', 'ghidra_out/labels_rtti_port_dtors.tsv'):
    for l in open(R + f, encoding='utf-8', errors='replace'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 2: lab.setdefault(p[0].lower(), p[1])
rb = collections.defaultdict(set)
for l in open(R + 'rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 2: rb[p[1].lower()].add(p[0])
ex = nf.exe()
def rd(a):
    for nm, base, d, ch in ex:
        if base <= a < base + len(d) - 3: return struct.unpack_from('<I', d, a - base)[0], nm
    return None, None
text = [(base, len(d)) for nm, base, d, ch in ex if nm.startswith('.text')]
for i in range(n):
    v, _ = rd(va + 4 * i)
    if v is None or not any(b <= v < b + L for b, L in text): break
    k = '%08x' % v
    print('%2d %08x %s | %s' % (i, v, lab.get(k, '-'), ' ; '.join(sorted(rb.get(k, ())))[:300]))
