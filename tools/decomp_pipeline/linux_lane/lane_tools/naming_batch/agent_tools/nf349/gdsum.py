# gdsum.py <pkdir> <objdir> <addr> [kinds]: per object kind (H, R, W, N), each ??$GetDef@ / ??$GetSubDef@ COMDAT the object defines: size (B)
# and pk_all verdict (from <pkdir>/<K>_<addr>.pkall.tsv), leaf-called flag. No bytes printed.
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
pkd, od, a = sys.argv[1:4]; kinds = (sys.argv[4] if len(sys.argv) > 4 else 'H R W N').split()
def objp(k): return '%s/%s%s%s.obj' % (od, 'H_' if k == 'H' else '', a, '_' + k.lower() if k in ('R', 'W', 'X') else '')
def pk(k):
    d = {}; p = '%s/%s_%s.pkall.tsv' % (pkd, k, a)
    if os.path.exists(p):
        for l in open(p):
            f = l.rstrip('\n').split('\t')
            if l.startswith('#') or len(f) < 10: continue
            d[f[9]] = (f[0], f[2], 'LC' if (f[7] == 'y' or f[8] == 'y') else '--')
    return d
for k in kinds:
    p = objp(k)
    if not os.path.exists(p): print(a, k, 'NOOBJ'); continue
    secs, sl = nf.coff(p); P = pk(k)
    for i, nm, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and (nm.startswith('??$GetDef@') or nm.startswith('??$GetSubDef@')) and secs[sec - 1]['ch'] & 0x1000:
            vv = P.get(nm, ('?', '?', '?'))
            print('%s %s %4d B %-8s %-9s %s %s' % (a, k, len(secs[sec - 1]['data']), vv[0][:8], vv[1], vv[2], nm[:110]))
