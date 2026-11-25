# vtcmp.py <obj> <vftable symbol> <retail vtable va hex>: the object's emitted vftable COMDAT, slot by slot (relocation symbol per
# 4-byte slot) against the retail vtable's slot targets labelled from ghidra_out/labels_rtti_port(_dtors).tsv and egor/fablewin PDB
# name sets by address. Prints per slot: SAME / DIFF / NOLABEL, and the slot-count comparison (retail count = slots until a target
# outside .text). No bytes printed.
import sys, struct, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429'
sys.path.insert(0, A + '/tools'); import nf
obj, vsym, va = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
secs, sl = nf.coff(obj)
sec = next(s for (i, n, v, s, t, c) in sl if n == vsym and s > 0)
s = secs[sec - 1]; rel = sorted(s['rel']); n = len(s['data']) // 4
mine = {o // 4: sym for o, sym, t in rel}
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
    for l in open('/home/user/FableDecomp/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
k = 0; out = []; same = diff = nol = 0
while True:
    t = rd(va + 4 * k)
    if t is None or not intext(t): break
    lab = L.get('%08x' % t, '-'); m = mine.get(k, '<none>')
    st = 'SAME' if lab == m else ('NOLABEL' if lab == '-' else 'DIFF')
    # NF429: retail _purecall (0x00BFEA08, no RTTI-port label) v an emitted pure slot; slot-0 `??_E` is VC7.1's weak alias of `??_G` (GOTCHAS nf425/nf427)
    if st == 'NOLABEL' and t == 0x00BFEA08 and m == '__purecall': st = 'SAME'; lab = '_purecall'
    if st == 'DIFF' and lab.startswith('??_G') and m == '??_E' + lab[4:]: st = 'SAME'
    if st == 'SAME': same += 1
    elif st == 'DIFF': diff += 1
    else: nol += 1
    out.append('slot %3d %08X %-8s ours=%s retail_label=%s' % (k, t, st, m, lab)); k += 1
print('\n'.join(out)); print('retail slots %d, emitted slots %d; SAME %d DIFF %d NOLABEL %d' % (k, n, same, diff, nol))
