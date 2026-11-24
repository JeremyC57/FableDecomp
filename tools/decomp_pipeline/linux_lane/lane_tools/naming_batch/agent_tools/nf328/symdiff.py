# symdiff.py <hd> <nd> <addr..>: per row, the DEF / UND external symbols only in HEAD's object and only in the new object (rename-map input)
import sys; sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
hd, nd = sys.argv[1:3]
for a in sys.argv[3:]:
    p = '/%s/%s/%s/%s.obj' % (a[:2], a[2:4], a, a); sh, sn = nf.ext_syms(hd + p), nf.ext_syms(nd + p)
    print('==', a)
    for x in sorted(sh - sn): print('  -', x)
    for x in sorted(sn - sh): print('  +', x)
