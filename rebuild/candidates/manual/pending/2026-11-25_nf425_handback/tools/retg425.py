#!/usr/bin/env python3
"""retg425.py <batch> <tag>: the scalar deleting destructor ??_G<C>@@UAEPAXI@Z each new CLONE_INLINE object newly emits (pick-any, reached
only through the vftable, so pk_all does not check it) compared with retail's vftable slot 0 body: bytes with relocation fields masked,
then each relocation target (REL32 destination / DIR32 value) against the symbol's addresses (tracker names + reloc_bindings).
Writes batches/<b>/evidence/retg.tsv (address, class, slot0 VA, slot0 landed self, verdict)."""
import sys, os, glob, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
import pe
W = L.A; b, tag = sys.argv[1:3]; B = W + '/batches/' + b; EXE = os.environ['FABLE_EXE']
S2A = collections.defaultdict(set)
for t, r in L.REC.items():
    for n in r.get('names', []): S2A[n].add(int(t, 16))
for s, v in L.TABW.items():
    for t, w in v: S2A[s].add(int(t, 16))
VT = collections.defaultdict(set)
for r in L.tsv(L.R + '/rebuild/manifest/rtti-vtables.tsv')[1:]: VT[r[0]].add(int(r[1], 16))
out = []
for a in open(B + '/rows.txt').read().split():
    C = L.WL[a]['klass']; o = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, tag, a, a))[0]
    secs, sl = L.nf.coff(o); k = None
    for i, nm, v, sec, t, scl in sl:
        if nm == '??_G%s@@UAEPAXI@Z' % C and sec > 0: k = sec
    if not k: out.append([a, C, '-', '-', 'NO_G_EMITTED']); continue
    vts = sorted(VT.get(C, []))
    if not vts: out.append([a, C, '-', '-', 'NO_RETAIL_VT']); continue
    s0 = struct.unpack('<I', pe.read_va(EXE, vts[0], 4))[0]
    d = bytearray(secs[k - 1]['data']); ret = bytearray(pe.read_va(EXE, s0, len(d))); bad = []
    for off, rs, typ in secs[k - 1]['rel']:
        r = struct.unpack_from('<I', ret, off)[0]
        tgt = (s0 + off + 4 + struct.unpack('<i', struct.pack('<I', r))[0]) & 0xffffffff if typ == 20 else r
        if tgt not in S2A.get(rs, set()): bad.append('%s->0x%08X' % (rs[:50], tgt))
        ret[off:off + 4] = b'\0' * 4; d[off:off + 4] = b'\0' * 4
    own = L.owner_self('%08x' % s0)[0]
    v = 'EQ' if ret == d and not bad else ('BYTES_DIFF' if ret != d else 'RELOC_DIFF ' + ' '.join(bad))
    out.append([a, C, '0x%08X' % s0, own or '-', v])
open(B + '/evidence/retg.tsv', 'w').write('address\tclass\tslot0\tslot0_landed_self\tverdict\n' + ''.join('\t'.join(x) + '\n' for x in out))
print(b, collections.Counter(x[4].split()[0] for x in out))
for x in out:
    if x[4] != 'EQ': print(' ', '\t'.join(x)[:300])
