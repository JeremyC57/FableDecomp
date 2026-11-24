# selfv.py <addr..>: a binder row's own Leaf, its Q->U / Q->M twin in each PDB name set, and every reloc_bindings line at its address
import sys, re, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334/tools'); import nf
R = '/home/user/FableDecomp'
P = {}
for nm in ('egor', 'fablewin'):
    st = set()
    for l in open(R + '/ghidra_out/%s_pdb_names.tsv' % nm, encoding='utf-8', errors='replace'):
        for f in l.rstrip('\n').split('\t'):
            if f.startswith('?'): st.add(f)
    P[nm] = st
T = collections.defaultdict(list)
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 3: T[p[1].lower().replace('0x', '')].append((p[0], p[2]))
for a in sys.argv[1:]:
    s = nf.src_of(a).read_text(encoding='utf-8', errors='replace'); m = re.search(r'^// Leaf:\s*(\S+)', s, re.M); lf = m.group(1) if m else '-'
    print('%s leaf=%s' % (a, lf))
    mm = re.match(r'(\?[^@]+@(?:[^@]+@)*@)([A-Z])(.*)$', lf)
    if mm:
        pre, acc, rest = mm.groups(); cands = set()
        for st in P.values():
            for x in st:
                if x.startswith(pre) and x.endswith(rest) and len(x) == len(lf): cands.add(x)
        for x in sorted(cands): print('   pdb: %s egor=%s fablewin=%s' % (x, x in P['egor'], x in P['fablewin']))
    for sy, w in T.get(a, []): print('   TAB %s (%s)' % (sy, w))
