# declsrc.py (NF329): every declaration genmodel wrote (work/model_<X>.tsv rows with action override / new / declare) for the classes in
# hdr/engine/: its RTTI-port label must be in egor_pdb_names.tsv and/or fablewin_pdb_names.tsv (RTTI placement + one PDB set = two
# sources, NF320 Q3). Prints class, slot, label, sets; NONE lines are decisions. Repo root.
import os, glob
A = os.environ['A']
sets = {}
for fn in ('egor', 'fablewin'):
    for l in open('ghidra_out/%s_pdb_names.tsv' % fn, encoding='utf-8', errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) > 1: sets.setdefault(q[1], set()).add(fn)
n = bad = 0
for h in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(h)[:-2]; mp = A + '/work/model_%s.tsv' % X
    if not os.path.exists(mp): print(X, 'NO_MODEL_TSV'); continue
    for l in open(mp).read().splitlines()[1:]:
        q = l.split('\t')
        if len(q) < 4 or q[3] not in ('override', 'new', 'declare'): continue
        s = sets.get(q[2], set()); n += 1
        if not s: bad += 1
        print('%s\t%s\t%s\t%s\t%s' % (X, q[0], q[3], q[2], '+'.join(sorted(s)) or 'NONE'))
print('TOTAL declarations %d, in neither PDB set %d' % (n, bad))
