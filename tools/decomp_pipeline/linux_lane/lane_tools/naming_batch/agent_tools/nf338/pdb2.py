# pdb2.py <idx..>: is each ENTRIES pdb_spelling an exact symbol in egor_pdb_names.tsv and fablewin_pdb_names.tsv
import sys, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); G = '/home/user/FableDecomp/ghidra_out/'
S = {}
for nm in ('egor', 'fablewin'):
    S[nm] = set(f for l in open(G + nm + '_pdb_names.tsv', encoding='utf-8', errors='replace') for f in l.rstrip('\n').split('\t')[1:])
E = [l.rstrip('\n').split('\t') for l in list(open(A + '/ENTRIES.tsv'))[1:]]
for i in map(int, sys.argv[1:]):
    p = E[i - 1][6]; print('%d\t%s\tegor=%s\tfablewin=%s\t%s' % (i, E[i - 1][0], p in S['egor'], p in S['fablewin'], p))
