# pkcmp346.py <pkdir> <addr...>: HEAD v new pick-any gate (NF340), both checkers, from <pkdir>/{H,N}_<addr>.{pkall.tsv,pvr.txt}.
# pk_all key (symbol, address), verdict column 1, leaf-called = leafref y or reach y; pvr key (symbol, address), EQ/NEQ, LEAF-REFERENCED.
# Per row and checker: TURNED_NEQ / NEWLY_NEQ (leaf-called -> GATE FAIL), FIXED, DROPPED, ADDED, and REMAIN_NEQ = every leaf-called NEQ
# GetDef / GetSubDef copy left in the NEW object (listed, not a gate failure: debt that was NEQ at HEAD). PKCMP_REVERSE=1 swaps the sides.
# Last lines: totals and GATE PASS/FAIL; a per-row summary line `ROW <addr> gate=<P|F> fixed_getdef=<n> fixed_subdef=<n> remain_getdef=<n> remain_subdef=<n>` (pk_all).
import sys, os, re
D = sys.argv[1].rstrip('/') + '/'
def pk(p):
    if not os.path.exists(p) or not open(p).readline().startswith('# leaf '): return None   # a crash / no-leaf run is MISSING, never SAME
    d = {}
    for l in open(p):
        if l.startswith('#') or not l.strip(): continue
        f = l.rstrip('\n').split('\t')
        if len(f) < 10: continue
        d[(f[9], f[2])] = (f[0], f[7] == 'y' or f[8] == 'y')
    return d
def pvr(p):
    if not os.path.exists(p) or 'Traceback' in open(p).read(): return None
    d = {}
    for l in open(p):
        f = l.split()
        if len(f) < 4 or f[0] not in ('EQ', 'NEQ'): continue
        d[(f[3], f[1])] = (f[0], 'LEAF-REFERENCED' in l)
    return d
def neq(v): return v is not None and v[0].startswith('NEQ')
def eq(v): return v is not None and v[0].startswith('EQ')
def kind(s): return 'getdef' if s.startswith('??$GetDef@') else 'subdef' if s.startswith('??$GetSubDef@') else 'other'
fails = 0; tot = {'pk_all': [0, 0, 0, 0], 'pvr': [0, 0, 0, 0]}
for a in sys.argv[2:]:
    rowfail = 0; summ = {}
    for name, fn, ext in (('pk_all', pk, 'pkall.tsv'), ('pvr', pvr, 'pvr.txt')):
        h, n = fn(D + 'H_%s.%s' % (a, ext)), fn(D + 'N_%s.%s' % (a, ext))
        if os.environ.get('PKCMP_REVERSE'): h, n = n, h
        if h is None or n is None: print(a, name, 'MISSING', 'H' if h is None else 'N'); fails += 1; rowfail += 1; continue
        out = []; fx = {'getdef': 0, 'subdef': 0, 'other': 0}; rem = {'getdef': 0, 'subdef': 0, 'other': 0}
        for k in sorted(set(h) | set(n)):
            hv, nv = h.get(k), n.get(k); lc = bool((hv and hv[1]) or (nv and nv[1]))
            if k[1] in ('-', ''): continue
            if hv and nv and eq(hv) and neq(nv): out.append(('TURNED_NEQ', k, lc)); tot[name][0] += 1; fails += lc; rowfail += lc
            elif hv is None and neq(nv): out.append(('NEWLY_NEQ', k, lc)); tot[name][1] += 1; fails += lc; rowfail += lc
            elif hv and nv and neq(hv) and eq(nv): out.append(('FIXED', k, lc)); tot[name][2] += 1; fx[kind(k[0])] += 1
            elif hv and nv is None: out.append(('DROPPED', k, lc)); tot[name][3] += 1
            elif nv and hv is None: out.append(('ADDED', k, lc))
            if nv and neq(nv) and lc and kind(k[0]) != 'other': out.append(('REMAIN_NEQ', k, lc)); rem[kind(k[0])] += 1
        for t, k, lc in out: print('%s\t%s\t%s\t%s\t%s\t%s' % (a, name, t, k[1], 'leaf-called' if lc else 'not-leaf-called', k[0]))
        if not out: print('%s\t%s\tSAME' % (a, name))
        summ[name] = (fx, rem)
    if 'pk_all' in summ:
        fx, rem = summ['pk_all']
        print('ROW %s gate=%s fixed_getdef=%d fixed_subdef=%d remain_getdef=%d remain_subdef=%d%s' % (a, 'F' if rowfail else 'P', fx['getdef'], fx['subdef'], rem['getdef'], rem['subdef'],
              '' if 'pvr' not in summ or summ['pvr'][0]['getdef'] == fx['getdef'] else ' pvr_fixed_getdef=%d' % summ['pvr'][0]['getdef']))
print('# totals [turned_NEQ, newly_NEQ, fixed, dropped]:', tot)
print('GATE', 'PASS' if not fails else 'FAIL (%d)' % fails)
