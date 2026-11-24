# tri332.py <idx..>: per ENTRIES line: PDB spelling, self Leaf, every reloc_bindings.tsv line at the target (full spelling + witness,
# flags), the exe census rows with stay-off / lock flags, and whether each PDB name set has the PDB spelling.
import sys, os, re, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); S = os.path.dirname(os.path.dirname(A)); R = '/home/user/FableDecomp'
why = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(A + '/work/offlimits_why.tsv')}
oe = set(open(A + '/work/other_agents_entries.txt').read().split()); ow = set(open(A + '/work/other_agents_witnesses.txt').read().split())
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')][1:]
C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
T = collections.defaultdict(list)
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 3: T[p[1].lower().replace('0x', '')].append((p[0], p[2].lower().replace('0x', '')))
def pdbsets():
    out = {}
    for nm in ('egor', 'fablewin'):
        st = set()
        for l in open(R + '/ghidra_out/%s_pdb_names.tsv' % nm, encoding='utf-8', errors='replace'):
            for f in l.rstrip('\n').split('\t'):
                if f.startswith('?'): st.add(f)
        out[nm] = st
    return out
P = pdbsets() if os.environ.get('NOPDB') != '1' else {'egor': set(), 'fablewin': set()}
def fl(a):
    f = []
    if a in why: f.append('OFF:' + why[a])
    if a in oe: f.append('NF333E')
    if a in ow: f.append('NF333W')
    o = S + '/w22/nflocks/%s/owner' % a
    if os.path.exists(o):
        w = open(o).read().strip()
        if w not in ('released', 'NF334'): f.append('LOCK=' + w)
        elif w == 'NF334': f.append('MINE')
    return a + ('[' + ','.join(f) + ']' if f else '')
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; a = e[0]; pdb = e[6]
    print('#%d %s %s %s  pdb_in: egor=%s fablewin=%s' % (i, fl(a), e[2], e[3], pdb in P['egor'], pdb in P['fablewin']))
    print('   PDB  ', pdb); print('   LEAF ', e[7])
    for s, w in T.get(a, []): print('   TAB%s %s  (%s)' % ('=' if s == pdb else '!', s, ' '.join(fl(x) for x in re.findall(r'[0-9a-f]{8}', w))))
    print('   census:', ' '.join(fl(x) for x in C.get(i, [])))
