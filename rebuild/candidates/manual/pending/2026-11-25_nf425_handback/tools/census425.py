#!/usr/bin/env python3
"""census425.py <batch...>: census of every NEW self symbol of the batches (selfmap col 3) and of every old self (col 2):
  - self_symbols.tsv lines naming it at another address; reloc_bindings.tsv lines (target, witness);
  - `// Leaf:` lines naming it in any other landed source; out-of-class definitions `C::Copy(` / `C::Clone(` in other landed sources;
  - in-class bodies of Copy / Clone in engine headers inside `class C` (a pick-any definer).
Writes batches/<b>/evidence/census.tsv (address, symbol, kind, hits) and prints counts. A NEW self hit elsewhere = a conflict to review;
an OLD self still named elsewhere = a caller binding to respell in the same batch."""
import sys, os, re, glob, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
R = L.R
srcs = sorted(glob.glob(R + '/rebuild/src/**/*.cpp', recursive=True)) + sorted(glob.glob(R + '/rebuild/tests/**/*.cpp', recursive=True))
TXT = {p: open(p, errors='replace').read() for p in srcs}
HDR = {p: open(p, errors='replace').read() for p in glob.glob(R + '/rebuild/include/**/*.h', recursive=True)}
SELF = collections.defaultdict(list)
for l in open(R + '/rebuild/oracles/self_symbols.tsv'):
    f = l.rstrip('\n').split('\t')
    if len(f) > 1 and f[1]: SELF[f[1]].append(f[0])
def own_files(a):
    s = L.nf.src_of(a); t = L.nf.test_of(a)
    return {str(s), str(t)}
jobs = []
for b in sys.argv[1:]:
    for l in open(L.A + '/batches/' + b + '/selfmap.tsv'):
        a, old, new = l.rstrip('\n').split('\t')
        for kind, sym in (('new', new), ('old', old)):
            m = re.match(r'\?(\w+)@(\w+)@@', sym); undec = re.sub(r'^_|@.*$', '', sym.lstrip('?'))
            pats = [re.escape(sym)]
            if kind == 'new' and m: pats.append(r'\b%s::%s\s*\(' % (m.group(2), m.group(1)))
            if kind == 'old': pats.append((r'\b%s\b' % re.escape(undec)) if not m else (r'\b%s::%s\b' % (m.group(2), m.group(1))))
            jobs.append((b, a, kind, sym, m, re.compile('|'.join(pats))))
# prefilter: one pass, files containing any of the identifiers
keys = set()
for b, a, kind, sym, m, rx in jobs:
    keys.add(m.group(2) if m else re.sub(r'^_|@.*$', '', sym.lstrip('?')))
big = re.compile('|'.join(sorted(re.escape(k) for k in keys)))
cand = collections.defaultdict(set)
for p, t in TXT.items():
    for k in set(big.findall(t)): cand[k].add(p)
res = collections.defaultdict(list)
for b, a, kind, sym, m, rx in jobs:
    mine = own_files(a); hits = []
    for x in SELF.get(sym, []):
        if x != a: hits.append('self_symbols:%s' % x)
    for tg, w in sorted(L.TABW.get(sym, ())):
        if tg != a: hits.append('table:%s(w %s)' % (tg, w))
    k = m.group(2) if m else re.sub(r'^_|@.*$', '', sym.lstrip('?'))
    for p in sorted(cand.get(k, ())):
        if p in mine: continue
        if rx.search(TXT[p]): hits.append('src:' + os.path.relpath(p, R + '/rebuild'))
    if kind == 'new' and m:
        C, fn = m.group(2), m.group(1)
        for p, t in HDR.items():
            if C not in t: continue
            for cm in re.finditer(r'^\s*class\s+%s\b[^;{]*\{' % re.escape(C), t, re.M):
                seg = t[cm.end(): cm.end() + 20000]; end = re.search(r'^\};', seg, re.M); seg = seg[:end.start()] if end else seg
                if re.search(r'\b%s\s*\([^;{]*\)\s*(const\s*)?\{' % fn, seg): hits.append('hdr-inline:' + os.path.relpath(p, R + '/rebuild/include'))
    res[b].append([a, kind, sym, str(len(hits)), ' '.join(hits[:12])])
for b in sys.argv[1:]:
    B = L.A + '/batches/' + b; out = res[b]
    with open(B + '/evidence/census.tsv', 'w') as f:
        f.write('address\tkind\tsymbol\thits\twhere\n')
        for x in out: f.write('\t'.join(x) + '\n')
    c = collections.Counter((x[1], x[3] != '0') for x in out)
    print(b, dict(c))
    for x in out:
        if x[3] != '0': print('  ', x[0], x[1], x[2][:90], x[4][:300])
