# wl334.py <idx..>: for each wrong table line of an ENTRIES target, the witness's non-comment source lines naming the WRONG spelling's
# token and the PDB token (STALE? = the wrong token no longer appears in the witness source). Screen only.
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
why = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(A + '/work/offlimits_why.tsv')}
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault(p[1].lower().replace('0x', ''), []).append((p[0], p[2].lower().replace('0x', '')))
def tok(s):
    m = re.match(r'\?\?([0-9])(\w+)@', s)
    if m: return ('~' if m.group(1) == '1' else '') + m.group(2)
    m = re.match(r'\?\?4', s)
    if m: return 'operator='
    m = re.match(r'\?\?\$(\w+)@', s) or re.match(r'\?(\w+)@', s) or re.match(r'[_@]?(\w+?)@\d+$', s) or re.match(r'_?(\w+)$', s)
    return m.group(1) if m else s
E = nf.entries()
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; t = e[0]; pdb = e[6]; pt = tok(pdb)
    print('#%d %s PDB %s' % (i, t, pdb[:200]))
    for s, w in T.get(t, []):
        if s == pdb: continue
        wt = tok(s); src = nf.src_of(w)
        flag = ('OFF:' + why[w]) if w in why else ''
        lk = nf.lock_owner(w)
        if lk not in ('released', 'NF338'): flag += ' LOCK=' + lk
        print('  W %s %s %s' % (w, flag, s[:220]))
        if flag.startswith('OFF') or not src or not src.exists():
            if not src or not src.exists(): print('     NOSRC')
            continue
        L = src.read_text(encoding='utf-8', errors='replace').split('\n'); hits = []
        for n, l in enumerate(L, 1):
            c = l.split('//')[0]
            if not c.strip(): continue
            if re.search(r'(?<![\w~])' + re.escape(wt.lstrip('~')) + r'\b', c) or re.search(r'(?<![\w~])' + re.escape(pt.lstrip('~')) + r'\b', c):
                hits.append('L%d: %s' % (n, c.strip()[:200]))
        for h in hits[:6]: print('     ' + h)
        if not hits: print('     (no token hit: %s / %s)' % (wt, pt))
