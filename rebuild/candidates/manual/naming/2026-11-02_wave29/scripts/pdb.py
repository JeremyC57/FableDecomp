# pdb.py <regex...>: PDB names matching (E = Ego_r, F = FableWin, D = Ego_d)
import sys, re
ROOT = '/home/user/FableDecomp/ghidra_out/'
H = {}
for tag, f in (('E', 'egor_pdb_names.tsv'), ('F', 'fablewin_pdb_names.tsv'), ('D', 'egod_pdb_names.tsv')):
    for l in open(ROOT + f, encoding='utf-8', errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: H.setdefault(q[1], set()).add(tag)
for pat in sys.argv[1:]:
    rx = re.compile(pat)
    for n in sorted(H):
        if rx.search(n): print(''.join(sorted(H[n])), n)
