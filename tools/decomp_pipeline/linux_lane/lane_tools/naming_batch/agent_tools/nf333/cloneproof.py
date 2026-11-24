# cloneproof.py <head.obj> <new.obj> <Class> <retail vtable va hex> <old self sym> <new self sym>
# The Clone-family objsec (NF320 Q1 ruling), allow-list form (NF323, 2026-11-24):
#  1. the Clone COMDAT: bytes equal, relocations equal by (offset, type, symbol) under old self -> new self;
#  2. DEF sets: HEAD defs (renamed) all present in new; new-only defs must be a subset of {??_G<C>, ??_E<C>};
#  3. the emitted ??_7<C>@@6B@: slot count == retail slot count, every slot's symbol == the retail RTTI-port label of that slot's
#     target (??_E<C> counts as ??_G<C>: VC7.1 weak alias), and the vftable section has no other relocations;
#  4. ??_G<C> body (when defined) v the retail slot-0 target: bytes equal with relocation fields zeroed, and each relocation's
#     retail target labelled (pickany-style);
#  5. .drectve: equal, or HEAD + exactly /DEFAULTLIB:"libcp" (NF318 Q1);
#  6. every other non-debug section equal (bytes + relocs) between the objects, matched by the COMDAT symbol it defines.
# Prints one PASS/FAIL line per check, then OK / NOT_OK. No bytes printed.
import sys, os, struct, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
H, N, C, VA, OLD, NEW = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4], 16), sys.argv[5], sys.argv[6]
VT = '??_7%s@@6B@' % C; G = '??_G%s@@UAEPAXI@Z' % C; E = '??_E%s@@UAEPAXI@Z' % C
mp = {OLD: NEW}
res = []; ok = True
def chk(name, cond, info=''):
    global ok
    res.append('%s %s %s' % ('PASS' if cond else 'FAIL', name, info)); ok = ok and cond
hs, hl = nf.coff(H); ns_, nl = nf.coff(N)
def secmap(sl, secs):   # COMDAT/defining symbol -> section index (first external def per section)
    m = {}
    for i, n, v, sec, typ, scl in sl:
        if scl == 2 and sec > 0 and v == 0: m.setdefault(n, sec)
    return m
hm, nm_ = secmap(hl, hs), secmap(nl, ns_)
def defs(sl): return {n for i, n, v, sec, typ, scl in sl if scl == 2 and sec > 0}
hd = {mp.get(x, x) for x in defs(hl)}; nd = defs(nl)
# 1 Clone section
if OLD in hm and NEW in nm_:
    a, b = hs[hm[OLD] - 1], ns_[nm_[NEW] - 1]
    ra = sorted((o, mp.get(s, s), t) for o, s, t in a['rel']); rb = sorted(b['rel'])
    chk('clone_bytes', a['data'] == b['data'], '%d v %d bytes' % (len(a['data']), len(b['data'])))
    chk('clone_relocs', ra == rb, '' if ra == rb else 'H=%s N=%s' % ([x[1] for x in ra], [x[1] for x in rb]))
else: chk('clone_found', False, 'old in H %s, new in N %s' % (OLD in hm, NEW in nm_))
# 2 DEF sets
lost = sorted(hd - nd); added = sorted(nd - hd)
chk('defs_none_lost', not lost, str(lost))
chk('defs_added_allowlist', set(added) <= {G, E}, str(added))
# 3 vftable
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; nsc = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; S_ = []
for i in range(nsc):
    snm = d[so + i * 40: so + i * 40 + 8].rstrip(b'\0').decode(); vsz, vaddr = struct.unpack_from('<II', d, so + i * 40 + 8); raw = struct.unpack_from('<I', d, so + i * 40 + 20)[0]
    S_.append((snm, base + vaddr, vsz, raw))
def rd(x, n=4):
    for snm, s0, n0, r in S_:
        if s0 <= x < s0 + n0: return d[r + x - s0: r + x - s0 + n]
def rd4(x):
    b = rd(x); return struct.unpack('<I', b)[0] if b else None
def intext(x): return any(snm == '.text' and s0 <= x < s0 + n0 for snm, s0, n0, r in S_)
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open('/home/user/FableDecomp/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
if VT in nm_:
    s = ns_[nm_[VT] - 1]; mine = {o // 4: sym for o, sym, t in s['rel']}; n = len(s['data']) // 4
    k = 0; bad = []
    while True:
        t = rd4(VA + 4 * k)
        if t is None or not intext(t): break
        lab = L.get('%08x' % t, '-'); m = mine.get(k, '<none>')
        if m == E: m = G
        if lab == E: lab = G   # dtor KIND is settled by the body check (4), not the donor label (deleting-destructor ruling)
        if lab != m: bad.append('slot %d ours=%s retail=%08X %s' % (k, m, t, lab))
        k += 1
    chk('vft_slotcount', k == n == len(mine), 'retail %d emitted %d relocs %d' % (k, n, len(mine)))
    chk('vft_slots', not bad, '; '.join(bad))
    slot0 = rd4(VA)
else:
    chk('vft_emitted', False, VT); slot0 = None
# 4 ??_G body v retail slot 0
if G in nm_ and slot0:
    s = ns_[nm_[G] - 1]; body = bytearray(s['data']); ret = bytearray(rd(slot0, len(body)) or b'')
    tg = []
    for o, sym, t in s['rel']:
        if t == 0x14: tgt = slot0 + o + 4 + struct.unpack_from('<i', ret, o)[0]
        elif t == 6: tgt = struct.unpack_from('<I', ret, o)[0]
        else: tgt = None
        tg.append('%s->%s(%s)' % (sym, '%08X' % tgt if tgt else '?', L.get('%08x' % tgt, '-') if tgt else '-'))
        for j in range(4): body[o + j] = 0; ret[o + j] = 0
    lab0 = L.get('%08x' % slot0, '-')
    chk('dtorG_bytes_v_retail', bytes(body) == bytes(ret), '%d bytes at %08X %s' % (len(body), slot0, lab0))
    T = {}
    for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
        r = l.rstrip('\n').split('\t')
        if len(r) == 3: T.setdefault(r[1].lower().replace('0x', ''), set()).add(r[0])
    relok = True
    for o, sym, t in s['rel']:
        if t == 0x14: tgt = slot0 + o + 4 + struct.unpack_from('<i', bytes(rd(slot0, len(body))), o)[0]
        else: tgt = struct.unpack_from('<I', bytes(rd(slot0, len(body))), o)[0]
        if sym != L.get('%08x' % tgt) and sym not in T.get('%08x' % tgt, set()): relok = False
    chk('dtorG_reloc_targets', relok, ' | '.join(tg))
elif G not in nm_: res.append('INFO no ??_G defined')
# 5 .drectve and 6 other sections
def dr(secs):
    x = [s for s in secs if s['name'] == '.drectve']; return x[0]['data'].decode('latin1').split() if x else []
th, tn = dr(hs), dr(ns_)
lib = sorted(tn) == sorted(th + ['/DEFAULTLIB:"libcp"'])
chk('drectve', th == tn or lib, 'equal' if th == tn else ('HEAD+libcp' if lib else 'H=%s N=%s' % (th, tn)))
skip = {mp.get(OLD, OLD), VT, G, E}
other = []
for sym, si in nm_.items():
    if sym in skip or sym.startswith('.'): continue
    hsym = {v: k for k, v in mp.items()}.get(sym, sym)
    if hsym not in hm: continue
    a, b = hs[hm[hsym] - 1], ns_[si - 1]
    if a['name'].startswith('.debug'): continue
    if a['data'] != b['data'] or sorted((o, mp.get(x, x), t) for o, x, t in a['rel']) != sorted(b['rel']): other.append(sym)
chk('other_comdats_equal', not other, str(other))
und_h = {n for i, n, v, sec, typ, scl in hl if scl == 2 and sec == 0}; und_n = {n for i, n, v, sec, typ, scl in nl if scl == 2 and sec == 0}
res.append('INFO und_added %d: %s' % (len(und_n - und_h), ' '.join(sorted(und_n - und_h))))
res.append('INFO und_dropped: %s' % ' '.join(sorted(und_h - und_n)))
print('\n'.join(res)); print('OK' if ok else 'NOT_OK')
