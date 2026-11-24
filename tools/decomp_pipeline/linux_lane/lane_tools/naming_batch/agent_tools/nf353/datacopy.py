# datacopy.py <batch-rows file> <census-rows file> <symbol...>: for COMDATs the batch rows drop that pk_all gives no address (vftables,
# ??_G / ??1 of STB and its base), compare each dropping row's HEAD copy (section bytes, relocation offsets / types / symbol names,
# COMDAT selection) with every outside census object's copy; prints per symbol and dropping row: the number of outside definers with an
# IDENTICAL copy (all / without the 9 holes) and a few names. Agent NF353.
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A)
holes = {l.split('\t')[0] for l in open(A + '/work/holes9.tsv') if l.strip()}
def comdat(p, sym):
    d = open(p, 'rb').read(); secs, sl = nf.coff(p)
    # selection from the section's aux record
    psym, nsym = struct.unpack_from('<II', d, 8)
    for i, nm, v, sec, t, scl in sl:
        if nm == sym and sec > 0 and scl == 2:
            x = secs[sec - 1]; sel = None
            for j, n2, v2, s2, t2, c2 in sl:
                if s2 == sec and c2 == 3 and n2 == x['name']:
                    o = psym + 18 * j; naux = d[o + 17]
                    if naux: sel = d[o + 18 + 14]
                    break
            return (x['data'], tuple((o_, s_, t_) for o_, s_, t_ in x['rel']), x['ch'], sel)
    return None
B = [l.strip() for l in open(sys.argv[1]) if l.strip()]; C = [l.strip() for l in open(sys.argv[2]) if l.strip()]
cc = {c: A + '/work/cobj/%s.obj' % c for c in C if os.path.exists(A + '/work/cobj/%s.obj' % c)}
for s in sys.argv[3:]:
    outs = {c: comdat(p, s) for c, p in cc.items()}
    for a in B:
        h = comdat(A + '/work/obj/H_%s.obj' % a, s); n = comdat(A + '/work/obj/N_%s.obj' % a, s)
        if h is None or n is not None: continue
        same = [c for c, v in outs.items() if v == h]; defs = [c for c, v in outs.items() if v is not None]
        print('%s\t%s\tsel=%s\tsize=%d\toutside_definers=%d\tidentical_all=%d\tidentical_no_holes=%d\t%s' % (
            s[:60], a, h[3], len(h[0]), len(defs), len(same), len([c for c in same if c not in holes]), ','.join([c for c in same if c not in holes][:6])))
