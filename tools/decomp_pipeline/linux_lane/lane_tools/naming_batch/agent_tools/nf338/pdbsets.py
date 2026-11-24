# pdbsets.py <idx...>: is each entry's pdb_spelling in egor_pdb_names.tsv and fablewin_pdb_names.tsv (exact string)? -> stdout tsv
import sys
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338'
sys.path.insert(0, A + '/tools'); import nf
G = '/home/user/FableDecomp/ghidra_out/'
def names(p):
    s = set()
    for l in open(G + p, encoding='utf-8', errors='replace'):
        for f in l.rstrip('\n').split('\t'):
            if f.startswith('?') or f.startswith('@'): s.add(f)
    return s
eg, fw = names('egor_pdb_names.tsv'), names('fablewin_pdb_names.tsv')
E = nf.entries()
for i in map(int, sys.argv[1:]):
    p = E[i - 1][6]; print('%d\t0x%s\t%s\tegor=%s\tfablewin=%s' % (i, E[i - 1][0].upper(), p, 'Y' if p in eg else 'N', 'Y' if p in fw else 'N'))
