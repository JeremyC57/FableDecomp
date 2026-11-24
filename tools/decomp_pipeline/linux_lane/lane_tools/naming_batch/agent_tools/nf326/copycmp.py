# copycmp.py (NF326): per unit member, every census object that DEFines the member's PDB symbol (objcensus_head.tsv) compared with the
# retail self bytes (objcensus.retail: relocation fields zeroed; each relocation's retail target and the symbol the object names) and with
# the new self draft's own COMDAT (cmpcomdat: bytes + relocation list). -> work/copycmp.tsv: member, obj, retail verdict, len, relocs, v_self
import sys, subprocess
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(A / 'tools')); import nf, objcensus
SELF = {'GetDefName': '004aa8d0', 'GetDataString': '004aa900', 'SetDataString': '004aa930', 'GetPos': '004aa980', 'GetAngleXY': '004aaa40',
        'MsgIsHitBySpecialAbilityFrom': '004aad20', 'MsgIsHitByAnySpecialAbilityFrom': '004aad40',
        'MsgIsHitByAnyAggressiveSpecialAbilityFrom': '004aad60', 'IsBeingCarriedBy': '004aafe0', 'IsNull': '004ab150',
        '??1': '004aa840', '??0': '004abe90'}
L = [l.rstrip('\n').split('\t') for l in open(A / 'work/objcensus_head.tsv')]
h = L[0]; out = open(A / 'work/copycmp.tsv', 'w')
import io, contextlib
def sec(obj, sym):
    secs, sl = nf.coff(obj)
    for i, n, v, s, t, scl in sl:
        if n == sym and s > 0 and scl == 2: x = secs[s - 1]; return x['data'], x['rel']
for j, w in enumerate(h[1:-1], 1):
    m = w[:3] if w.startswith('??') else w[1:].split('@')[0]
    selfobj = A / 'work/obj' / (SELF[m] + '.obj')
    ref = sec(str(selfobj), w) if selfobj.exists() else None
    for r in L[1:]:
        if not r[j].startswith('DEF'): continue
        o = str(A / 'work/hobj' / r[0])
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf): objcensus.retail(w, SELF[m], [o])
        p = buf.getvalue().strip().split('\t')
        x = sec(o, w)
        vs = '-' if ref is None else ('SELF_EQUAL' if x == ref else 'SELF_DIFF bytes=%s relocs=%s' % (x[0] == ref[0], x[1] == ref[1]))
        out.write('\t'.join([m, r[0][:8], r[j], p[1], p[2], p[3] if len(p) > 3 else '', vs]) + '\n')
out.close()
