# definers.py: per-symbol definer table for the CThingNoiseDef unit under each option: every object that defines the symbol, its COMDAT
# selection, size, and whether its copy equals the reference copy of 0x0045C5F6's HEAD object (every function there is retail-EQ by
# retcmp, both vftables 25-slot EQ by vtcmp). Copy identity = bytes + relocation symbol names (stand-in names for the same retail address
# are mapped first). A strong (sel 1) definer beside any other definer is an LNK2005; differing pick-any copies link silently (first wins).
import sys, os, hashlib
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = nf.W
SYN = {'??0CMemberBase_0044c0c0@@QAE@XZ': '??0CSubDefClassBase@@QAE@XZ', '??0CShoveHaPennyRegionDef@@QAE@XZ': '??0CSubDefClassBase@@QAE@XZ'}
SYMS = ['?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ', '??0CThingNoiseDef@@QAE@XZ', '??0CTCNoiseDef@@QAE@XZ', '??_7CThingNoiseDef@@6B@',
        '??_7CTCNoiseDef@@6B@', '??_GCThingNoiseDef@@UAEPAXI@Z', '??1CThingNoiseDef@@UAE@XZ', '??_GCTCNoiseDef@@UAEPAXI@Z', '??1CTCNoiseDef@@UAE@XZ']
import struct
def copies(path):
    d = open(path, 'rb').read(); psym, nsym = struct.unpack_from('<II', d, 8)
    secs, sl = nf.coff(path); sel = {}; i = 0
    while i < nsym:
        o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8)
        if scl == 3 and v == 0 and na == 1 and sec > 0 and sec not in sel: sel[sec] = d[o + 18 + 14]
        i += 1 + na
    out = {}
    for i, n, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and n in SYMS:
            s = secs[sec - 1]; rel = tuple((o, SYN.get(x, x), ty) for o, x, ty in s['rel'])
            out[n] = (sel.get(sec, '-'), len(s['data']), hashlib.sha1(s['data'] + repr(rel).encode()).hexdigest()[:10])
    return out
SETS = {
 'HEAD': ['hobj/0045c5f6', 'hobj/00455ef0', 'hobj/0044c1bb', 'hobj/0045c5da', 'hobj/00749810', 'hobj/00749a70', 'hobj/00455f3c'],
 'A_allocform': ['hobj/0045c5f6', 'exp/obj/c1_allocform_455ef0', 'exp/obj/d1_allocform_44c1bb', 'exp/obj/e1_allocform_45c5da', 'hobj/00749810', 'hobj/00749a70', 'hobj/00455f3c'],
 'O_outofline': ['hobj/0045c5f6', 'exp/obj/c2_ool_455ef0', 'exp/obj/d2_ool_44c1bb', 'hobj/0045c5da', 'hobj/00749810', 'hobj/00749a70', 'hobj/00455f3c'],
 'N_nf86_one_row': ['hobj/0045c5f6', 'exp/obj/c3_nf86_455ef0', 'hobj/0044c1bb', 'hobj/0045c5da', 'hobj/00749810', 'hobj/00749a70', 'hobj/00455f3c'],
 'S_one_source': ['hobj/0045c5f6', 'hobj/00749810', 'hobj/00749a70', 'hobj/00455f3c'],
}
ref = copies(str(W / 'hobj/0045c5f6.obj'))
for name, objs in SETS.items():
    print('######## option %s: %s' % (name, ' '.join(os.path.basename(o) for o in objs)))
    C = {o: copies(str(W / (o + '.obj'))) for o in objs}
    for s in SYMS:
        defs = [(os.path.basename(o), C[o][s]) for o in objs if s in C[o]]
        strong = sum(1 for _, c in defs if c[0] == 1)
        dup = 'LNK2005' if strong and len(defs) > 1 else '-'
        bodies = len({c[2] for _, c in defs})
        print('%-48s definers=%d strong=%d %-10s distinct_bodies=%d  %s' % (s, len(defs), strong, dup, bodies,
              ' | '.join('%s sel=%s %dB %s' % (o, c[0], c[1], 'EQ' if s in ref and c[2] == ref[s][2] else 'NEQ') for o, c in defs)))
