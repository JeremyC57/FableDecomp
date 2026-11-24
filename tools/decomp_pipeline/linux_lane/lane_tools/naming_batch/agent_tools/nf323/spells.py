# spells.py <idx..>: full spellings of an ENTRIES target: PDB, self Leaf, every reloc_bindings line (symbol, witness),
# each with the first differing position against the PDB spelling and the differing tail (truncation-free; NF306 lesson)
import sys, os, re
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')][1:]
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault(p[1].lower().replace('0x', ''), []).append((p[0], p[2]))
def d(a, b):
    k = 0
    while k < min(len(a), len(b)) and a[k] == b[k]: k += 1
    return k
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; t = e[0]; pdb = e[6]
    print('#%d %s PDB %s' % (i, t, pdb))
    sl = e[7]; k = d(sl, pdb)
    print('   self_leaf %s' % ('== PDB' if sl == pdb else '@%d: %s' % (k, sl[max(0, k - 20):])))
    for s, w in T.get(t, []):
        k = d(s, pdb)
        print('   table [%s] %s' % (w, '== PDB' if s == pdb else '@%d: ...%s' % (k, s[max(0, k - 20):])))
    if not T.get(t): print('   (no table lines for %s)' % t)
