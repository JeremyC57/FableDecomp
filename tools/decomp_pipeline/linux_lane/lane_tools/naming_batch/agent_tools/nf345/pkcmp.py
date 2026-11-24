# pkcmp.py <addr...>: HEAD v new for the coordinator's gate, from evidence/pk/{H,N}_<addr>.{pkall.tsv,pvr.txt}.
# pk_all: key (symbol, address); verdict = column 1 (first token, _AMBIG kept), leaf-called = leafref y or reach y.
# pvr: key (symbol, address); verdict EQ/NEQ; leaf-called = LEAF-REFERENCED.
# Reports per row: turned NEQ (EQ->NEQ), newly NEQ (absent at HEAD, NEQ new), fixed (NEQ->EQ), dropped / added copies, and the
# PKCMP_REVERSE=1 swaps the sides (planted control).
# GetDef<CMaterialDef> (0x0066C340) copy verdict. GATE FAIL if any leaf-called copy turned / is newly NEQ, or a 0x0066C340 copy is not EQ.
import sys, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); D = A + '/evidence/pk/'
def pk(p):
    d = {}
    if not os.path.exists(p): return None
    for l in open(p):
        if l.startswith('#') or not l.strip(): continue
        f = l.rstrip('\n').split('\t')
        if len(f) < 10: continue
        d[(f[9], f[2])] = (f[0], f[7] == 'y' or f[8] == 'y')
    return d
def pvr(p):
    d = {}
    if not os.path.exists(p): return None
    for l in open(p):
        f = l.split()
        if len(f) < 4 or f[0] not in ('EQ', 'NEQ'): continue
        d[(f[3], f[1])] = (f[0], 'LEAF-REFERENCED' in l)
    return d
def neq(v): return v is not None and v[0].startswith('NEQ')
def eq(v): return v is not None and v[0].startswith('EQ')
fails = 0; tot = {'pk_all': [0, 0, 0, 0, 0], 'pvr': [0, 0, 0, 0, 0]}
for a in sys.argv[1:]:
    for name, fn, ext in (('pk_all', pk, 'pkall.tsv'), ('pvr', pvr, 'pvr.txt')):
        h, n = fn(D + 'H_%s.%s' % (a, ext)), fn(D + 'N_%s.%s' % (a, ext))
        if os.environ.get('PKCMP_REVERSE'): h, n = n, h   # planted control: new as HEAD, HEAD as new -> the fixes must read TURNED_NEQ
        if h is None or n is None: print(a, name, 'MISSING', 'H' if h is None else 'N'); fails += 1; continue
        out = []
        for k in sorted(set(h) | set(n)):
            hv, nv = h.get(k), n.get(k); lc = bool((hv and hv[1]) or (nv and nv[1]))
            if k[1] in ('-', ''): continue
            if hv and nv and eq(hv) and neq(nv): out.append(('TURNED_NEQ', k, lc)); tot[name][0] += 1; fails += lc
            elif hv is None and neq(nv): out.append(('NEWLY_NEQ', k, lc)); tot[name][1] += 1; fails += lc
            elif hv and nv and neq(hv) and eq(nv): out.append(('FIXED', k, lc)); tot[name][2] += 1
            elif hv and nv is None: out.append(('DROPPED', k, lc)); tot[name][3] += 1
            elif nv and hv is None: out.append(('ADDED', k, lc))
            if k[1].lower() == '0066c340' and nv is not None:
                out.append(('MATERIALDEF_COPY ' + nv[0], k, lc)); fails += not eq(nv); tot[name][4] += eq(nv)
        for tag, k, lc in out:
            print('%s\t%s\t%s\t%s\t%s\t%s' % (a, name, tag, k[1], 'leaf-called' if lc else 'not-leaf-called', k[0]))
        if not out: print('%s\t%s\tSAME' % (a, name))
print('# totals [turned_NEQ, newly_NEQ, fixed, dropped, 0x0066C340 EQ copies]:', tot)
print('GATE', 'PASS' if not fails else 'FAIL (%d)' % fails)
