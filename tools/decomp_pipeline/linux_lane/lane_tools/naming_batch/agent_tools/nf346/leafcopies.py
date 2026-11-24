# leafcopies.py <obj> <leaf-symbol> <retail-addr> <retail-len>: every function COMDAT the object defines that the leaf function calls / jumps to
# (rel32 relocations of the leaf section), mapped to its retail target through the retail bytes at the leaf's own relocation sites, and compared
# with Fable.exe at that target: bytes with every relocation field zeroed on both sides, and each relocation's retail target read from the exe
# must be bound in reloc_bindings.tsv to the symbol the copy names (or the copy's own symbol for a self-reference; or, for a string literal
# COMDAT ??_C@ the object defines, the retail target must hold the same bytes). Prints EQ / NEQ per copy.
import sys, os, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
obj, leaf, ra, rl = sys.argv[1], sys.argv[2], int(sys.argv[3], 16), int(sys.argv[4])
X = nf.exe()
def rd(va, n):
    for nm, base, d, ch in X:
        if base <= va < base + len(d): return d[va - base: va - base + n]
secs, sl = nf.coff(obj)
defs = {}
for i, n, v, sec, t, scl in sl:
    if scl == 2 and sec > 0: defs.setdefault(n, (sec, v))
T = collections.defaultdict(set)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
    r = l.rstrip('\n').split('\t')
    if len(r) == 3: T[r[1].lower()].add(r[0])
ls, lv = defs[leaf]; L = secs[ls - 1]; retail = rd(ra, rl)
called = collections.OrderedDict()
for off, sym, typ in L['rel']:
    if typ != 0x14 or sym == leaf or sym not in defs: continue
    o = off - lv
    tgt = (ra + o + 4 + struct.unpack_from('<i', retail, o)[0]) & 0xffffffff
    called.setdefault(sym, set()).add(tgt)
neq = 0
for sym, tg in called.items():
    sec, v = defs[sym]; s = secs[sec - 1]
    if not s['name'].startswith('.text'): continue
    for t in sorted(tg):
        body = bytearray(s['data']); ret = bytearray(rd(t, len(body)) or b'')
        bad = []; lits = []
        for off, rs, ty in s['rel']:
            if off + 4 > len(ret): bad.append('reloc past end'); continue
            if ty == 0x14: rt = (t + off + 4 + struct.unpack_from('<i', ret, off)[0]) & 0xffffffff
            elif ty == 6: rt = struct.unpack_from('<I', ret, off)[0]
            else: rt = None
            body[off:off + 4] = b'\0' * 4; ret[off:off + 4] = b'\0' * 4
            ok = rt is not None and (rs in T.get('%08x' % rt, set()) or (rs == sym and rt == t) or rs.startswith('$L'))
            if not ok and rt is not None and rs.startswith('??_C@') and rs in defs:   # string literal COMDAT: same text at the retail target
                lit = secs[defs[rs][0] - 1]['data']; ok = rd(rt, len(lit)) == lit
                if ok: lits.append(rs[:40])
            if not ok: bad.append('reloc +%x %s -> retail %s' % (off, rs[:70], '%08x' % rt if rt is not None else '?'))
        beq = bytes(body) == bytes(ret)
        v_ = 'EQ' if beq and not bad else 'NEQ'
        neq += v_ == 'NEQ'
        print('%s\t%08x\t%d\tbytes_equal=%s\t%s\t%s' % (v_, t, len(body), beq, sym, '; '.join(bad[:3]) + (' literal-text-equal: ' + ','.join(lits) if lits else '')))
print('TOTAL copies=%d NEQ=%d' % (sum(len(x) for x in called.values()), neq))
