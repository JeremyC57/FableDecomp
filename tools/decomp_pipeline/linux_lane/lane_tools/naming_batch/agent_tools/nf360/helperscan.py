# helperscan.py <symbols file> <obj...>: for each dropped helper symbol: (1) reloc_bindings.tsv lines naming it (symbol column, exact);
# (2) in every given object, relocations in non-debug sections OUTSIDE the helper's own COMDAT section that name it, and UND entries for it.
# Prints per symbol `<sym> table_lines=<n> refs=<n> und=<n>` plus each hit; last line: objects scanned, total refs/und.
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
syms = [l.rstrip('\n') for l in open(sys.argv[1]) if l.strip()]
tab = {}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
    f = l.rstrip('\n').split('\t')
    if f and f[0] in syms: tab[f[0]] = tab.get(f[0], 0) + 1
hits = {s: [] for s in syms}; und = {s: [] for s in syms}; objs = sys.argv[2:]
S = set(syms)
for o in objs:
    secs, sl = nf.coff(o)
    own = {}
    for i, n, v, sec, t, scl in sl:
        if n in S and sec > 0 and scl == 2: own[n] = sec
        if n in S and sec == 0 and scl == 2: und[n].append(os.path.basename(o))
    for k, s in enumerate(secs, 1):
        if s['name'].startswith('.debug'): continue
        for off, sym, t in s['rel']:
            if sym in S and own.get(sym) != k: hits[sym].append('%s sec %d +%x' % (os.path.basename(o), k, off))
tr = tu = 0
for s in syms:
    print('%s table_lines=%d refs=%d und=%d' % (s, tab.get(s, 0), len(hits[s]), len(und[s])))
    for h in hits[s] + ['UND ' + x for x in und[s]]: print('    ' + h)
    tr += len(hits[s]); tu += len(und[s])
print('# objects scanned %d, symbols %d, table lines %d, references %d, UND %d' % (len(objs), len(syms), sum(tab.values()), tr, tu))
